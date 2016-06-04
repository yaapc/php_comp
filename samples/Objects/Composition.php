<?php

  class Screw
  {
      int $size;
      string $type;
    
      function Screw(int $size,string $type){
        $this->size = $size;
        $this->type = $type;
      }
      
      public function toString() : string {
        return "Screw {$size,$type}";
      }
  }

  class Engine
    {  
        int $cylinders;
        Screw $screw;
      
        function Engine(int $cylinders){
          $this->cylinders = $cylinders;
          $screw = new Screw(10,"Slot");
        }    
      
      
        public function toString() : string {   
          return "Engine {"+$screw->toString() +","+ $cylinders+"} ";
        }
    }

    class Car
    {      
        string $color = "Pink";
        Engine $engine;
        function Car(){
          $engine = new Engine(2);
        }
        
        public function toString() : string {   
          return "Car {"+$engine->toString() +","+ $color+"}";
        }
    }
  
  Car $c = new Car();
  
  echo $c->toString();
?>