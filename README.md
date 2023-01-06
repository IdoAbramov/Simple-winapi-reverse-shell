# Simple-winapi-reverse-shell
Simple windows reverse shell using WinAPI

*** made for self-learning purpose only ***

Use netcat as server-attacker listining in port 8081 (or w/e you change in the client's code).

Once executed, the victim's client connects to the server.

The std i/o piped to the socket so the attacker gets the cmd of the victim.
