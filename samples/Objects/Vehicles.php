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
        echo "MotorBike can move !!";
      }
      
      function toString() : string {
        return "MotorBike";
      }
  }
  
  class Track extends Vehicle{
   
      function  Track(){}
      
      function move() : void {
        echo "Track can move !!";
     }
     
       function toString() : string {
        return "Track";
      }
   }
  
   class Bicycle extends Vehicle{
   
      function  Bicycle(){}
      function move() : void {
        echo "Bicycle can move !!";
     }
     
       function toString() : string {
        return "Bicycle";
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
    echo $v;
    $i+=1;
    echo "------------------------------------------";
  }
?>