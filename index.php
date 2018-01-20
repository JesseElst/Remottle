<html>
	<head>
	<title>Retrieve data from database </title>
        <link rel="stylesheet" type="text/css" href="main.css">
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.1/jquery.min.js"></script>
        
	</head>
	<body>
        <!-- Het overzicht van het aantal mililiters per dag-->
 <table id="customers">
     <thead><tr>
         <th>Datum</th>
    <th>Aantal milli</th>
    
 </tr>
         </thead>
	<?php
    //gegevens databse
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "water";

// maar verbinding met database
$conn = new mysqli($servername, $username, $password, $dbname);
// check vebinding met database
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
     
//query voor overzicht
$sql = "
SELECT sum(water) AS Hoeveelheid, DATE(tijd) as date 
FROM fles
GROUP BY DATE(date)
ORDER BY date DESC
LIMIT 0 , 30";
     
//wuery voor aantal mililiter vandaag
$sqlelse = "
SELECT sum(water) AS Hoeveelheid FROM `fles` WHERE DATE(`tijd`) = CURDATE() ORDER BY tijd;
";

//resultaten database hoeveel water per dag
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo "<tr> <th>" . $row["date"]. "</th> <th>". $row["Hoeveelheid"]. "</th> </tr>";
      }
} else {
    echo "0 results";
}
 ?>   
</table>
        
  <script> 
    //Hoeveel water gedronken vandaag  
       var water= 
           <?php
           $result = $conn->query($sqlelse);
           if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        
        echo $row["Hoeveelheid"];
        
    }
} else {
    echo "0 results";
}

  
?>;
       //hoe groot moet de grafiek worden
      var procent = 0 + water * 0.05;
      //hoeveel moet er nog gedronken worden
      var tegaan = 2000 - water;
        </script>

        <!-- Fles visualisatie-->
        <section id="fles">
          <center> <h2>2000 ML</h2> </center>
        <section id="water">
            <center> <script> document.write(water + ' ML Gedronken'); </script>
                </center>
            </section>
        </section>
        <script>
     
    // animatie fles        
$(document).ready(function(){
    $('#water').animate({
        height: procent + "%"
        
        
    
    }, 2000)
})
</script>
         <!-- visualisatie hoeveel nog tegaan-->
        <section id="meter">
            <h1> Nog <script>document.write(tegaan + ' ML te gaan!'); </script></h1>
        </section>
	</body>
	</html>
	