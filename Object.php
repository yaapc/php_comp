<?php

class Object {
  int     $id  = 0;
  string  $tag = "Object";
  function Object(){}
  
  function to_string() : void {
    echo $tag+"@"+$id;
  }
}

?>