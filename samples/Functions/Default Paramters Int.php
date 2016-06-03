<?php
	function funcInt(int $a = 1, int $b = 2,int $c = 3) : void {
		echo $a,"\t",$b,"\t",$c;
		echo "\n";
	}
		  
	funcInt();

	funcInt(54);
	  
	funcInt(10,145);
	 
	funcInt(3,2,1);
?>