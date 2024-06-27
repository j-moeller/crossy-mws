package com.crossy.app;

import java.nio.charset.StandardCharsets;

public class App 
{
    public static void main(String[] args)
    {
        GsonTarget gsonTarget = new GsonTarget();
        String templateString = "{\"int-to-float\": 10, ";

        int len = 66;
        for (int i = 0; i < len; i++) {
            templateString += "\"" + i + "\": 1";
            for (int j = 0; j < i; j++) {
                templateString += "0";
            }

            if (i < len - 1) {
                templateString += ",";
            }
        }

        templateString += "}";

        byte[] result = gsonTarget.run(templateString.getBytes(StandardCharsets.UTF_8));
        System.out.println(new String(result, StandardCharsets.UTF_8));
    }
}
