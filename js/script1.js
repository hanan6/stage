

angular.module("mdp-v", [])
.controller("mdp-c", function($scope){
	
         
     

      $scope.texte= function(){
      		$.get("index.php",function(rep) {
             $scope.charger=rep;
        });
      }







});
  








