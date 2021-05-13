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

### Connection
When new client is connected, server calls *c_init_client()* to give new cliet it's id and confirm successful connection. After that each layer on board must be added on client's canvas. At the end of inicialization client notified about completing all init operations with call *c_finish_board_init()*.