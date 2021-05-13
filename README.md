Blackboard server
=================
Server for online board for working with [clients](https://github.com/MaxMalts/Online_Board). In current version communication between them is carried out using JSON format. Message is consists of three parts:
- Size of header in 8 bytes as *uint64_t*
- Header in JSON format:
```
{
  int: client_id,
  string: method,
  int: argument_size,
}
```
- Arguments in JSON format, the number and type of which depends on the method.

API description
---------------
### Server API includes next methods with corresponding arguments:
- ***s_add_layer***
```
{
  int layer_id,
  double[2]: position,
  double: scale,
  string: layer_type,
  object: layer_data
}
```
- ***s_clear_board***  
`No arguments`
- ***s_delete_layer***
```
{
  int: layer_id
}
```
- ***s_undo***  
`No arguments`

### Client API includes next methods with corresponding arguments:
- ***c_init_client***
```
{
  int: client_id,
}
```
- ***c_finish_board_init***  
`No arguments`
- ***c_add_layer***
```
{
  int layer_id,
  double[2]: position,
  double: scale,
  string: layer_type,
  object: layer_data
}
```
- ***c_confirm_add_layer***
```
{
  int: layer_id
}
```
- ***c_clear_board***  
`No arguments`
- ***c_delete_layer***
```
{
  int: layer_id
}
```

Server-client communication schemes
-----------------------------------
### Connecting
When new client is connected, server calls *c_init_client()* to give new cliet it's id and confirm successful connection. After that each layer on board must be added on client's canvas. At the end of inicialization client notified about completing all init operations with call *c_finish_board_init()*.
![Connecting new client scheme](https://github.com/egor79k/blackboard_server/blob/master/img/connecting.png)

### Creating layer
When client creates new layer it calls *s_add_layer()* following which server calls *c_add_layer()* for all other clients except layer's creator and confirms successful adding with calling *c_confirm_add_layer()* for creator. Confirm is necessary to give layer's creator new layer's unique id, that could be generated just on server.
![Adding new layer scheme](https://github.com/egor79k/blackboard_server/blob/master/img/creating_layer.png)

### Deleting layer
When client deletes one of layers it calls *s_delete_layer()* with layer's id following which server calls *c_delete_layer()* for all other clients except layer's deleter.

### Canceling last action
When client calls *s_undo()* to cancel it's last action server defines the type of last operation made by this client and calls corresponding methods for all clients (including caller). For example if last operation was adding layer then for all clients would be called *c_delete_layer()*.

### Clearing board
When client calls *s_clear_board()*, server clears it's canvas and calls the similar method *c_clear_board()* for all other clients except caller.
