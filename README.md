# Compile and run the server
Lancer le serveur sur le port 5001
```bash
gcc server.c -o server && ./server 5001
```

# Compile and run the client
Run the client on port 4001 and connect to serveur 5001
```bash
gcc client.c -o client && ./client 4001 5001
```

> Warning
> This is a localhost only serveur client
