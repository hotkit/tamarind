# # Very simple workflow

# `ingress` shows the data needed to start the workflow. Example data points follow
path = ingress "/path/to/api"
# `GET` can be used to simulate a HTTP request. Return status and example data values
# follow
response = GET "http://api.example.com${path}/" 200 {"example": "data"}
# `await` is used to wait for asynchronous data (like from `GET` and other HTTP requests
await response:
    # `schema-check` will check values against a schema
    schema-check response.body {"properties": {"example": {"const": "data"}}}
    # The exit data from the workflow
    exit
