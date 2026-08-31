A C++ program that checks to see that Proxmox nodes are running, and sends alerts to a discord webhook.

Requires the following environment variables to be set:

PROXMOX_SECRET\
The API Token secret for your proxmox token.

AUTH_PREFIX
The prefix to the secret that creates the full Authorization header. For example:\
PVEAPIToken=user@realm!name=

DISCORD_WEBHOOK\
The URL to your discord webhook

PROXMOX_API_URL\
The URL to your proxmox dashboards node api. For example:\
https://192.168.0.10:8006/api2/json/nodes
