# API Documentation
## User Management
```POST /api/signup```

Register a new user account.

- Request Body:

username: Username string
password: Password string
other user info fields
- Response:

201: User created successfully
409: Username already exists

```POST /api/login```

Login with username and password.

- Request Body:

username: Username string
password: Password string
- Response:

200: Login succeeded
401: Invalid credentials
Chat

```POST /api/chat```

Send a chat message.

- Request Body:

content: Chat message content
userId: Sender user ID
roomId: Chat room ID
- Response:

200: Message sent
404: User or chat room not found
Friend Management
POST /api/nfmgr
Manage friend requests.

- Request Body:

action: add to send request, accept to accept request
userId: Recipient user ID
- Response:

200: Request success
404: User not found
GET /api/newfriends
Get list of pending friend requests.

- Response:
200: Array of friend request objects
401: Unauthorized
User Info
GET /api/info
Get user profile information.

- Request Header:

Authorization: JWT token
- Response:

200: User info object
401: Invalid token
File Management
POST /api/file/save
Save a file.

- Request Body:

file: File data
path: File path
filename: Filename
- Response:

201: File saved
500: Error saving file
GET /api/file/get
Get a file.

- Request Parameter:

filePath: Path of file on server
Response:

200: File data
404: File not found