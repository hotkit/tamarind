user = ingress username
lists = GET "https://lists/${user1}/" 200 {"lists": []}
await lists:
    show lists
    exit
