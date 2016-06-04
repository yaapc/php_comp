<?php

class Object {
  int     $ObjectID  = 0;
  string  $ClassTAG = "Object";
  function Object(){}
  
  public function toString() : string {
    return $ClassTAG+"@"+$ObjectID;
  }
}

?>