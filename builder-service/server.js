const http = require("http");
const fs = require("fs");
const path = require("path");
const crypto = require("crypto");

const PORT = process.env.PORT || 10000;
const OWNER = process.env.GITHUB_OWNER || "Sukumar002";
const REPO = process.env.GITHUB_REPO || "zmk-keyboards";
const WORKFLOW = process.env.GITHUB_WORKFLOW || "custom-build.yml";
const BRANCH = process.env.GITHUB_BRANCH || "main";
const TOKEN = process.env.GITHUB_TOKEN;
const BUILDER_KEY = process.env.BUILDER_ACCESS_KEY || "";
const BUILDER_KEYS = (process.env.BUILDER_ACCESS_KEYS || "")
  .split(",")
  .map((value) => value.trim())
  .filter(Boolean);

const INDEX = path.resolve(__dirname, "index.html");
const API = "https://api.github.com";
const allowedBoards = new Set(["hana","chocofi","sofle","corne"]);
const allowedConnectivity = new Set(["ble","dongle"]);
const allowedMcu = new Set(["nice_nano","xiao_ble"]);
const allowedDisplay = new Set(["none","128x32","128x64"]);

function send(res, status, body, headers = {}) {
  const data = typeof body === "string" ? body : JSON.stringify(body);
  res.writeHead(status, {
    "Content-Type": typeof body === "string" ? "text/plain; charset=utf-8" : "application/json; charset=utf-8",
    "Cache-Control": "no-store",
    ...headers
  });
  res.end(data);
}

function requireConfig(res) {
  if (!TOKEN) {
    send(res, 503, { error: "Server is missing GITHUB_TOKEN." });
    return false;
  }
  if (!BUILDER_KEY && BUILDER_KEYS.length === 0) {
    send(res, 503, { error: "Server is missing builder access keys." });
    return false;
  }
  return true;
}

function safeEqual(a, b) {
  if (typeof a !== "string" || typeof b !== "string") return false;
  const aBuf = Buffer.from(a);
  const bBuf = Buffer.from(b);
  return aBuf.length === bBuf.length && crypto.timingSafeEqual(aBuf, bBuf);
}

function authorized(req) {
  const supplied = req.headers["x-builder-key"] || "";
  if (BUILDER_KEY && safeEqual(supplied, BUILDER_KEY)) return true;
  return BUILDER_KEYS.some((key) => safeEqual(supplied, key));
}

async function github(url, options = {}) {
  return fetch(url, {
    ...options,
    headers: {
      "Accept": "application/vnd.github+json",
      "Authorization": `Bearer ${TOKEN}`,
      "X-GitHub-Api-Version": "2022-11-28",
      ...(options.headers || {})
    }
  });
}

async function readJson(req) {
  let body = "";
  for await (const chunk of req) {
    body += chunk;
    if (body.length > 100000) throw new Error("Request too large");
  }
  return body ? JSON.parse(body) : {};
}

function sanitizeLabel(value) {
  return String(value || "")
    .toLowerCase()
    .replace(/[^a-z0-9._-]+/g, "-")
    .replace(/^-+|-+$/g, "")
    .slice(0, 60);
}

async function findRun(requestId) {
  const url = `${API}/repos/${OWNER}/${REPO}/actions/workflows/${encodeURIComponent(WORKFLOW)}/runs?event=workflow_dispatch&per_page=30`;
  const r = await github(url);
  if (!r.ok) throw new Error(`GitHub runs lookup failed (${r.status})`);
  const data = await r.json();
  return (data.workflow_runs || []).find(run =>
    String(run.display_title || run.name || "").includes(requestId)
  ) || null;
}

async function findFinalArtifact(runId) {
  const r = await github(`${API}/repos/${OWNER}/${REPO}/actions/runs/${runId}/artifacts?per_page=100`);
  if (!r.ok) throw new Error(`GitHub artifact lookup failed (${r.status})`);
  const data = await r.json();
  const artifacts = (data.artifacts || []).filter(a => !a.expired && !String(a.name || "").startsWith("raw-"));
  return artifacts[0] || null;
}

const server = http.createServer(async (req, res) => {
  try {
    const url = new URL(req.url, `http://${req.headers.host || "localhost"}`);

    if (req.method === "GET" && (url.pathname === "/" || url.pathname === "/index.html")) {
      const html = fs.readFileSync(INDEX, "utf8");
      res.writeHead(200, {"Content-Type":"text/html; charset=utf-8","Cache-Control":"no-cache"});
      return res.end(html);
    }

    if (req.method === "GET" && url.pathname === "/api/health") {
      return send(res, 200, { ok: true, githubConfigured: Boolean(TOKEN), accessKeyConfigured: Boolean(BUILDER_KEY || BUILDER_KEYS.length) });
    }

    if (url.pathname.startsWith("/api/")) {
      if (!requireConfig(res)) return;
      if (!authorized(req)) return send(res, 401, { error: "Invalid builder access code." });
    }

    if (req.method === "POST" && url.pathname === "/api/build") {
      const body = await readJson(req);
      const board = String(body.board || "");
      const connectivity = String(body.connectivity || "");
      const split_mcu = String(body.split_mcu || "");
      const dongle_mcu = String(body.dongle_mcu || "nice_nano");
      const dongle_display = connectivity === "dongle" ? String(body.dongle_display || "none") : "none";
      const build_label = sanitizeLabel(body.build_label);

      if (!allowedBoards.has(board) || !allowedConnectivity.has(connectivity) ||
          !allowedMcu.has(split_mcu) || !allowedMcu.has(dongle_mcu) ||
          !allowedDisplay.has(dongle_display)) {
        return send(res, 400, { error: "Invalid build selection." });
      }

      const requestId = crypto.randomUUID();
      const dispatch = await github(`${API}/repos/${OWNER}/${REPO}/actions/workflows/${encodeURIComponent(WORKFLOW)}/dispatches`, {
        method: "POST",
        headers: {"Content-Type":"application/json"},
        body: JSON.stringify({
          ref: BRANCH,
          inputs: {
            board,
            connectivity,
            split_mcu,
            dongle_mcu,
            dongle_display,
            build_label,
            request_id: requestId
          }
        })
      });

      if (!dispatch.ok) {
        const detail = (await dispatch.text()).slice(0, 500);
        return send(res, 502, { error: `GitHub dispatch failed (${dispatch.status}).`, detail });
      }

      return send(res, 202, { request_id: requestId });
    }

    const statusMatch = url.pathname.match(/^\/api\/build\/([^/]+)\/status\/?$/i);
    if (req.method === "GET" && statusMatch) {
      const requestId = decodeURIComponent(statusMatch[1]).trim();
      const run = await findRun(requestId);
      if (!run) return send(res, 200, { state: "queued" });

      if (run.status !== "completed") {
        return send(res, 200, { state: run.status || "in_progress", run_id: run.id });
      }
      if (run.conclusion !== "success") {
        return send(res, 200, { state: "failed", conclusion: run.conclusion, run_id: run.id });
      }

      const artifact = await findFinalArtifact(run.id);
      if (!artifact) return send(res, 200, { state: "packaging", run_id: run.id });

      return send(res, 200, {
        state: "ready",
        run_id: run.id,
        artifact_name: artifact.name,
        download_url: `/api/build/${requestId}/download`
      });
    }

    const dlMatch = url.pathname.match(/^\/api\/build\/([^/]+)\/download\/?$/i);
    if (req.method === "GET" && dlMatch) {
      const requestId = decodeURIComponent(dlMatch[1]).trim();
      const run = await findRun(requestId);
      if (!run || run.status !== "completed" || run.conclusion !== "success") {
        return send(res, 409, { error: "Firmware is not ready yet." });
      }
      const artifact = await findFinalArtifact(run.id);
      if (!artifact) return send(res, 404, { error: "Final firmware artifact not found." });

      const r = await github(`${API}/repos/${OWNER}/${REPO}/actions/artifacts/${artifact.id}/zip`, { redirect: "follow" });
      if (!r.ok) return send(res, 502, { error: `Artifact download failed (${r.status}).` });

      const bytes = Buffer.from(await r.arrayBuffer());
      res.writeHead(200, {
        "Content-Type": "application/zip",
        "Content-Disposition": `attachment; filename="${artifact.name}.zip"`,
        "Content-Length": bytes.length,
        "Cache-Control": "no-store"
      });
      return res.end(bytes);
    }

    send(res, 404, { error: "Not found.", method: req.method, path: url.pathname });
  } catch (err) {
    console.error(err);
    send(res, 500, { error: "Internal server error." });
  }
});

server.listen(PORT, "0.0.0.0", () => {
  console.log(`ZMK builder service listening on port ${PORT}`);
});
