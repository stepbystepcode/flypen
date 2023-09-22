# API Documentation
---
title: flypen v1.0.0
language_tabs:
  - shell: Zsh
  - http: HTTP
  - javascript: JavaScript
toc_footers: []
includes: []
search: true
code_clipboard: true
highlight_theme: darkula
headingLevel: 2

---

# flypen

## POST /api/signup

Register a new user account.

POST /api/signup

### Request Body

| Name | Type | Description |
| --- | --- | --- |
| username | string | Username |
| password | string | Password |

### Responses

| HTTP Status Code | Meaning | Description | Data schema |
| --- | --- | --- | --- |
| 201 | User created successfully |  |  |
| 409 | Username already exists |  |  |

## POST /api/login

Login with username and password.

POST /api/login

### Request Body

| Name | Type | Description |
| --- | --- | --- |
| username | string | Username |
| password | string | Password |

### Responses

| HTTP Status Code | Meaning | Description | Data schema |
| --- | --- | --- | --- |
| 200 | Login succeeded |  |  |
| 401 | Invalid credentials |  |  |

## POST /api/avatar

Set user avatar.

POST /api/avatar

### Request Body

| Name | Type | Description |
| --- | --- | --- |
| avatar | int | Avatar ID |

### Responses

| HTTP Status Code | Meaning | Description | Data schema |
| --- | --- | --- | --- |
| 200 | Avatar set successfully |  |  |
| 401 | Invalid credentials |  |  |

## POST /api/chat

Send a chat message.

POST /api/chat

### Request Body

| Name | Type | Description |
| --- | --- | --- |
| content | string | Chat message content |
| receiver | string | Receiver username |

### Responses

| HTTP Status Code | Meaning | Description | Data schema |
| --- | --- | --- | --- |
| 200 | Message sent |  |  |
| 404 | User or chat room not found |  |  |

## POST /api/check

Check if a user is online.

POST /api/check

### Responses

| HTTP Status Code | Meaning | Description | Data schema |
| --- | --- | --- | --- |
| 200 | User is online |  |  |
| 404 | User not found |  |  |

## POST /api/nfmgr

Manage friend requests.

POST /api/nfmgr

### Request Body

| Name | Type | Description |
| --- | --- | --- |
| info | string | "add" or "accept" |
| username | string | Recipient username |

### Responses

| HTTP Status Code | Meaning | Description | Data schema |
| --- | --- | --- | --- |
| 200 | Request success |  |  |
| 404 | User not found |  |  |

## GET /api/newfriends

Get list of pending friend requests.

GET /api/newfriends

### Responses

| HTTP Status Code | Meaning | Description | Data schema |
| --- | --- | --- | --- |
| 200 | Array of friend request objects |  | Array of friend request objects |
| 401 | Unauthorized |  |  |

## GET /api/info

Get user profile information.

GET /api/info

### Request Headers

| Name | Type | Description |
| --- | --- | --- |
| Authorization | string | JWT token |

### Responses

| HTTP Status Code | Meaning | Description | Data schema |
| --- | --- | --- | --- |
| 200 | User info object |  | User info object |
| 401 | Invalid token |  |  |

## POST /api/file/save

Save a file.

POST /api/file/save

### Request Body

| Name | Type | Description |
| --- | --- | --- |
| file | binary | File data |
| path | string | File path |
| filename | string | Filename |

### Responses

| HTTP Status Code | Meaning | Description | Data schema |
| --- | --- | --- | --- |
| 201 | File saved |  |  |
| 400 | Invalid request |  |  |
| 500 | Error saving file |  |  |

## GET /api/file/get

Get a file.

GET /api/file/get

### Request Parameters

| Name | Type | Description |
| --- | --- | --- |
| filePath | string | Path of file on server |

### Responses

| HTTP Status Code | Meaning | Description | Data schema |
| --- | --- | --- | --- |
| 200 | File data |  | File data |
| 404 | File not found |  |  |

## POST upload

POST /api/upload

> Body Parameters

```yaml
image: string

```

### Params

|Name|Location|Type|Required|Description|
|---|---|---|---|---|
|body|body|object| no |none|
|» image|body|string(binary)| yes |none|


### Responses

|HTTP Status Code |Meaning|Description|Data schema|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|Success|Inline|

### Responses Data Schema


## POST commands

POST /api/commands

> Body Parameters

```json
{
  "commands": "1",
  "params": [
    "",
    ""
  ]
}
```

### Params

|Name|Location|Type|Required|Description|
|---|---|---|---|---|
|body|body|object| no |none|
|» commands|body|string| yes |none|
|» params|body|[string]| yes |none|

> Response Examples

> 200 Response

```json
{}
```

### Responses

|HTTP Status Code |Meaning|Description|Data schema|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|Success|Inline|

### Responses Data Schema

## POST lock

POST /api/file/lock

### Params

|Name|Location|Type|Required|Description|
|---|---|---|---|---|
|filename|query|string| yes |none|

> Response Examples

> 200 Response

```json
{}
```

### Responses

|HTTP Status Code |Meaning|Description|Data schema|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|Success|Inline|

### Responses Data Schema

