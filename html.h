#ifndef _HTML_H_
#define _HTML_H_

String htmlMainpage = "<!DOCTYPE html>\
<html lang=\"ja\">\
    <head>\
        <script>\
            function ledOn(){\
                fetch(\"./led\", {\
                    method: \"POST\",\
                    cache: \"no-cache\",\
                    body: {\"led0\":\"on\"}\
                })\
            }\
        </script>\
    </head>\
    <body>\
        <button onclick=\"ledOn\">LEDON</button>\
        <form action=\"./lighton\">\
            <input type=\"submit\" value=\"TEST1\">\
        </form>\
    </body>\
</html>";


#endif