#!/usr/bin/env bash
# Firewall: allow only SSH, HTTP, HTTPS (run as root on the server)
ufw default deny incoming; ufw default allow outgoing
ufw allow 22/tcp; ufw allow 80/tcp; ufw allow 443/tcp
ufw --force enable; ufw status verbose
