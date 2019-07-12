//Must output the website with no external refs, just plain html...
//Use % % % (with no spaces) to represent a line converted, it will be repeated
#include "output.h"

void outputSite(void (*printFunction)(const char *) ) {
	printFunction("<!doctype html>");
	printFunction("");
	printFunction("<html lang=\"en\">");
	printFunction("<head>");
	printFunction("    <meta charset=\"utf-8\">");
	printFunction("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">");
	printFunction("    <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css\" integrity=\"sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO\" crossorigin=\"anonymous\">");
	printFunction("    <title>CRW System Monitor</title>");
	printFunction("    <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
	printFunction("    <script src=\"https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.3/umd/popper.min.js\" integrity=\"sha384-ZMP7rVo3mIykV+2+9J3UJ46jBk0WLaUAdn689aCwoqbBJiSnjAK/l8WvCWPIPm49\" crossorigin=\"anonymous\"></script>");
	printFunction("    <script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js\" integrity=\"sha384-ChfqqxuZUCnJSK3+MXmPNIyE6ZbWh2IMqE241rYiqJxyMiZ6OW/JmZQ5stwEULTy\" crossorigin=\"anonymous\"></script>");
	printFunction("    <!--    <script src=\"//cdn.rawgit.com/Mikhus/canvas-gauges/gh-pages/download/2.1.5/radial/gauge.min.js\"></script> -->");
	printFunction("");
	printFunction("    <script>");
	printFunction("        function loadIt() {");
	printFunction("            $.getJSON(\"/data.json\", function (data) {");
	printFunction("                $(\"#dTemp\").html(data.Temp + \" F\");");
	printFunction("            });");
	printFunction("            $.get(\"/log\", function (data) {");
	printFunction("                $(\"#dLog\").html(data);");
	printFunction("            });");
	printFunction("        }");
	printFunction("");
	printFunction("        $(document).ready(function () {");
	printFunction("            loadIt();");
	printFunction("            setInterval(loadIt, 5000);");
	printFunction("        });");
	printFunction("");
	printFunction("    </script>");
	printFunction("</head>");
	printFunction("<!-- test -->");
	printFunction("<body>");
	printFunction("    <div class=\"container\">");
	printFunction("        <div class=\"jumbotron\">");
	printFunction("            <h1>Remote Sensor, Cooler Control</h1>");
	printFunction("        </div>");
	printFunction("");
	printFunction("        <div class=\"well\">Current Temperature</div>");
	printFunction("        <div id=\"dTemp\">xx.x</div>");
	printFunction("");
	printFunction("        <div>Log File</div>");
	printFunction("        <div id=\"dLog\"></div>");
	printFunction("    </div>");
	printFunction("</body>");
	printFunction("</html>");
}
