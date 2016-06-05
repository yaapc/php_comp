<?php
  class Vehicle{
  
     function  Vehicle(){}
     
      function move() : void {
        echo "Vehicles can move !!";
      }
  }
  
  class MotorBike extends Vehicle{
  
      function  MotorBike(){}
      
      function move() : void {
        echo $this;
        echo "MotorBike can move !!";
      }
  }
  
  class Track extends Vehicle{
   
      function  Track(){}
      
      function move() : void {
        echo $this;
        echo "Track can move !!";
     }
   }
  
   class Bicycle extends Vehicle{
   
      function  Bicycle(){}
      function move() : void {
        echo $this->toString();
        echo "Bicycle can move !!";
     }
  }
  
  
  Vehicle $v;
  
  int $i = 0;
  while($i<3)
  {
    if ($i == 0)
    {
      $v = new MotorBike();
    }
    
    if ($i == 1)
    {
      $v = new Track();
    }
    
    if ($i == 2)
    {
      $v = new Bicycle();
    }
    
    $v->move();
    $i+=1;
    echo "------------------------------------------";
  }
?>
