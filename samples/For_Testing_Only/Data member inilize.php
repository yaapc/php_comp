<?php
  
class TEST{
  function TEST(){}
  
  int $intVar = 10;

  float $floatVar = 1.555;

  bool $boolVar   = true;

  string $stringVar = "HaHa";

}

  TEST $x = new TEST();

  echo $x->intVar;
  echo $x->floatVar;
  echo $x->boolVar ;
  echo $x->stringVar;

  $x->floatVar  = 1550.00;
  $x->intVar    = 324;
  $x->stringVar = "HeHe";

  echo "_____________________";

  echo $x->intVar;
  echo $x->floatVar;
  echo $x->boolVar ;
  echo $x->stringVar;
?>
------------------------------------------------------------------------------------
10
1.555
1
HaHa

324
1550.00
1
HeHe