<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
		
		
		<title>MDP</title>
		<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css">
		<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap-theme.min.css">
		<link rel="stylesheet" href="style.css">
	</head>
	<body>
		<center>
		<div class="container">
		<p class="spacer">
		<form  action="index.php" method="post" enctype="multipart/form-data"  >
			<input type="hidden" name="MAX_FILE_SIZE" value="400000" />
		<table ><tr><td><button name="exemple1"  class="btn btn-default">exemple1</button> </td> <td><button name="exemple2"  class="btn btn-default">exemple2</button></td>  <td><button name="exemple3"  class="btn btn-default">exemple3</button> </td></tr></table>
		
		
		
		<table >
			
			<tr><td><textarea name="textarea" rows="10" cols="50" class="form-control" ><?php $value ?></textarea></td></tr>
			<tr> <td><input type="submit" name="run" value="run mdp"   class="btn btn-default" /></td></tr>
		</table>
	</form></p></div></center>
	
	

	
</body>
</html>
