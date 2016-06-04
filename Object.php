<?php

class Object {
  int     $id  = 0;
  string  $tag = "Object";
  function Object(){}
  
  function toString() : string {
    return $tag+"@"+$id;
  }
}

?>