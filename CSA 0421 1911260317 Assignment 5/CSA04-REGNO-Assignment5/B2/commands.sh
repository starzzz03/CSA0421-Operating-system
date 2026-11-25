# Run these on a machine with Docker (not available in the authoring sandbox)
docker build -t result-portal . && docker images result-portal
docker run -d --name portal -p 8000:8000 result-portal && docker ps && docker exec portal whoami   # expect: portal
curl -s localhost:8000/health
docker run -d --name limited --cpus 0.5 --memory 128m -p 8001:8000 result-portal && docker stats --no-stream
docker history result-portal
docker build -f Dockerfile.multistage -t result-portal:multi . && docker build -f Dockerfile.ubuntu -t result-portal:ubuntu . && docker images result-portal
docker compose up -d && docker compose ps && docker stats --no-stream
