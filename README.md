Blackboard server
=================
Server for online board for working with [clients](https://github.com/MaxMalts/Online_Board)

API description
---------------
Server API includes next methods:
- s_add_layer
- s_clear_board
- s_delete_layer
- s_undo

Client API includes next methods:
- c_init_client
- c_finish_board_init
- c_add_layer
- c_confirm_add_layer
- c_clear_board
- c_delete_layer

### Connecting
When new client is connected, server calls *c_init_client()* to give new cliet it's id and confirm successful connection. After that each layer on board must be added on client's canvas. At the end of inicialization client notified about completing all init operations with call *c_finish_board_init()*.

### Creating layer
When client creates new layer it calls *s_add_layer()* following which server calls *c_add_layer()* for all other clients except layer's creator and confirms successful adding with calling *c_confirm_add_layer()* for creator. Confirm is necessary to give layer's creator new layer's unique id, that could be generated just on server.

### Deleting layer
When client deletes one of layers it calls *s_delete_layer()* with layer's id following which server calls *c_delete_layer()* for all other clients except layer's deleter.

### Canceling last action
When client calls *s_undo()* to cancel it's last action server defines the type of last operation made by this client and calls corresponding methods for all clients (including caller). For example if last operation was adding layer then for all clients would be called *c_delete_layer()*.

### Clearing board
When client calls *s_clear_board()*, server clears it's canvas and calls the similar method *c_clear_board()* for all other clients except caller.
