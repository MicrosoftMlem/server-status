# server-status

A C++ program that checks to see that Proxmox nodes are running, and sends alerts to a discord webhook.\

The program runs on a systemd timer which can be configured to run every 20 minutes or similar.
It sends a timestamped alert to the provided Discord webhook url, showing the status and uptime of your Proxmox nodes.\

---

## Dependencies\

- [nlohmann/json](https://github.com/nlohmann/json)
- [cpr](https://github.com/libcpr/cpr)

---

## Building

### Arch

Install `nlohmann/json` from the official repos:\
```bash
sudo pacman -S nlohmann/json
```
cpr comes from the AUR:\
```bash
yay -S cpr
```

Build with CMake:

```bash
cmake -S . -B build
cmake --build build
```

---

## Running

To run a single check, immediately:\

```bash
./build/server-status
```

It can be scheduled with a SystemD timer for periodic monitoring.

---

Requires the following environment variables to be set:

PROXMOX_SECRET\
The API Token secret for your proxmox token.

AUTH_PREFIX\
The prefix to the secret that creates the full Authorization header. For example:\
PVEAPIToken=user@realm!name=

DISCORD_WEBHOOK\
The URL to your discord webhook

PROXMOX_API_URL\
The URL to your Proxmox node's API endpoint. For example:b\
`https://192.168.0.10:8006/api2/json/nodes`
