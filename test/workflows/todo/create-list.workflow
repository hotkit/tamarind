user = ingress "testuser1" "testuser2"
todo = input {"title": "string", "slug": "string"}
list = PUT "https://todo/${user}/${todo.slug}" 200 {"title" : "${todo.title}", "items": []}
await list:
    exit
