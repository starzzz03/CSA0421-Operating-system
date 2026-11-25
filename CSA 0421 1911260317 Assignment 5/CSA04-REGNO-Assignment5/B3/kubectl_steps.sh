# On minikube/kind: eval $(minikube docker-env); docker build -t simats/result-portal:1.0 ../B2
kubectl apply -f deployment.yaml; kubectl get pods
kubectl delete pod <name>; kubectl get pods -w          # a new pod appears
kubectl describe pod <name> | grep -A6 -i -E "limits|requests"
kubectl scale deployment result-portal --replicas=6; kubectl get hpa
