<?php

  class ABC{
	  public int $x;
	  function ABC(int $xValue =150){
		  $x = $xValue;
	  }
  }
  
  function ABCIncremnt(ABC $abc) : ABC {
	  $abc->x += 150;
	  return $abc;
  }
  
  ABC $abc = new ABC();
  
  $abc = ABCIncremnt($abc);
  
  echo $abc->x;
	
?>