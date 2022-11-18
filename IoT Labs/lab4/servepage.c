void servepage ( void )
{
    String reply;

    reply += "<!DOCTYPE HTML>";
    reply += "<html>";
    reply += "<head>";
    reply += "<title>Dr Nock's sensor</title>";
    reply += "</head>";
    reply += "<body>";
    reply += "<h1>This is Dr Nock's wireless sensor node</h1>";

    reply += "You can add content here to represent various system parameters etc";
    reply += "</body>";
    reply += "</html>";
    
    SERVER.send(200, "text/html", reply);
}
