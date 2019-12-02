# Very simple workflow

path = ingress
request = GET http://api.example.com/${path}/ 200 {"example": "data"}
await request:
    schema-check ${request} {"properties": {"example": {"const": "data"}}}
    exit request
