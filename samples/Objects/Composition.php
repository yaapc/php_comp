<?php

  class Screw
  {
      int $size;
      string $type;
    
      function Screw(int $screwSize,string $screwType){
        $size = $screwSize;
        $type = $screwType;
      }
      
      function to_string() : string {
        return $size + " " + $type;
      }
  }

  class Engine
    {  
        int $cylinders;
        Screw $screw;
      
        function Engine(int $cCount){
          $cylinders = $cCount;
          $screw = new Screw(10,"Slot");
        }    
      
      
        function to_string() : string {   
          return $screw->to_string() + " " + $cylinders;
        }
    }

    class Car
    {      
        string $color = "Pink";
        Engine $engine;
        function Car(){
          $engine = new Engine(2);
        }
        
        function to_string() : string {   
          return $engine->to_string() + " " + $color;
        }
    }
  
  Car $c = new Car();
  
  echo $c->to_string();
?>