angular.module('mdp-v', [])
.controller('mdp-c', ['$scope', '$http', 
  function($scope, $http) {
    $scope.method = 'GET';
    // ------------------Debut - On spécifie l url de notre fichier PHP--------
    $scope.url = 'http://localhost/stage/php/ctrl.php';
    // ------------------Fin - On spécifie l url de notre fichier PHP--------


    // ------------------Debut - Fonction appellée lors du click sur le button Exemple1--------
    $scope.fetch = function() {

      
      $http({method: $scope.method, url: $scope.url,}).
        success(function(data) {
          
          $scope.data = data;
          // alert (data);
        
        }).
        error(function(data, status) {
          $scope.data = data || "Request failed";
          $scope.status = status;
      });


    };
    // ------------------Fin - Fonction appellée lors du click sur le button Exemple1--------

    

    // ------------------Debut - Fonction qui permet de vider le TextArea--------
    $scope.clear = function() {
       document.getElementById('mytextarea').value = "";
       //location.reload();
       document.location.reload(true);
    };

    // ------------------Fin - Fonction qui permet de vider le TextArea--------


    // ------------------Debut - Fonction qui permet de lancer lors du click sur le button run mdp--------

             $scope.run = function() {
              
            $scope.texte= document.getElementById('mytextarea').value ;
            var request = $http({method: "post", url:'http://localhost/stage/php/run.php',data : {texte : $scope.texte}});

            request.success(function (data) {
             alert(data);
              }); 

                                     };
     // ------------------Fin - Fonction du run mdp--------


    
     

  }]);





