user = ingress "testuser1" "testuser2"
lists = GET "https://todo/${user1}/" 200 {"lists": []}
await lists:
    show lists
    exit
