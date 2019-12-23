user = ingress "testuser1" "testuser2"
todo = input:
    title = string
    slug = /[a-zA-Z0-9%-]+/:
        format filename-part
list = PUT "https://todo/${user}/${todo.slug}" 200:
    {"title" : "${todo.title}", "items": []}
    {"title": "${todo.title}", "items": [
        {},
        {}]}
await list:
    goto view-todo-list {"slug": ${todo.slug},
        "todo": ${todo}}
