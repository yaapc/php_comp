Expected Output:
A -> C
A -> B
C -> B
A -> C
B -> A
B -> C
A -> C
=====================================================================
Real Output:

<?php
    function hanoi(string $a,string $b,string $c,int $n = 3) : void {
    if ($n > 1)
    {
      hanoi($a,$c,$b,$n-1);
      echo $a," -> ",$c;
      hanoi($b,$a,$c,$n-1);
    }
    else{
     echo $a," -> ",$c;
    }
  }
  
  hanoi("A","B","C");
?>
