<?php
		
   $postdata = file_get_contents("php://input");
    $request = json_decode($postdata);
    @$texte = $request->texte;
   
    



$monfichier= fopen("../MDP/instances/LEAP/leap_1.fasta",'w');
fwrite($monfichier,$texte); 	



system("../MDP/./build/MDP2 -i ../MDP/instances/LEAP/leap_1.fasta -o ../MDP/results/leap_1.xml -D 99 2>&1");


		
?>
