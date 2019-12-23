list = ingress:
    {"user": "testuser1", "slug": "list1"}
    {"user": "testuser2", "todo": {
        "title": "Example todo list",
        "slug": "example-todo-list",
        "items": []}}
await list:
    # schema-check ${list}
    opt schema-check ${list} {"properties": {
            "slug": "string"}}:
        t = GET "https://todo/${list.user}/${list.slug}"
        await t:
            list.todo = ${t.body}
    else:
        schema-check ${list} {"properties": {
            "todo": "object"}}
    show ${list.todo.title}
