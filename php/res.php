<?php 

$pdo= new PDO('mysql::host=localhost;dbname=resultat','root','ce1mdpp');
$requete_result=$pdo->query("SELECT id_result FROM motif ORDER BY id_result DESC LIMIT 1 ;");
$res=$requete_result->fetch(PDO::FETCH_NUM);
$resul=$res[0];
$requete_pattern=$pdo->prepare("select count(bloc.patron) from bloc,motif,solution where solution.motif=motif.motif and motif.id_bloc =bloc.id_bloc and solution.id_ins= ? and solution.id_result=$resul and motif.id_result=$resul and bloc.id_result=$resul ;");
$nb=$requete_pattern->fetch(PDO::FETCH_NUM);
$nbp=$nb[0]; 

echo "<center><table><tr>
      <td>Instance</td>
      <td>Classe</td>
      <td>Taille</td>
      <td>Modele</td>
      <td>Nombre Patterns</td>
      <td>Patterns</td>
      <td>Sequences</td>
      </tr>";

//--------------------------------------------------------------------------

$instance="LEAP";

$xml=simplexml_load_file("../MDP/results/leap_1.xml");

foreach($xml->class as $class)

{

$classe= $class["name"];
$taille= $class->nbPos["val"];
$nb_p=$requete_pattern->execute($classe);

$requete_pattern=$pdo->query("select count(bloc.patron) from bloc,motif,solution where solution.motif=motif.motif and motif.id_bloc =bloc.id_bloc and solution.id_ins= $classe and solution.id_result=$resul and motif.id_result=$resul and bloc.id_result=$resul ;");
$nb=$requete_pattern->fetch(PDO::FETCH_NUM);
$nbp=$nb[0]; 

echo "<tr><td>$instance</td><td>$classe</td> <td>$taille</td>
        <td><a href=#signet>Modele</a></td>
       <td>$nbp</td> <td><a href=#signet>Patterns</a></td> <td><a href=#signet>Sequences</a></td>

      </tr></center>";



};


echo "</table>";
?>
