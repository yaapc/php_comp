<?php
  require_once('Integer.php');
  Object $o1  = new Object();
  Object $o2  = new Object();
  Object $o3  = $o1;
  
  if ($o1->equal($o1)){
		echo $o1," Equal ",$o2;
  }else{
		echo $o1," Not Equal ",$o2;
  }	  
	
?>