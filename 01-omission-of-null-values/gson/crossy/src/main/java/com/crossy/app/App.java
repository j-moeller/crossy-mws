package com.crossy.app;

import java.nio.charset.StandardCharsets;

public class App 
{
    public static void main(String[] args)
    {
        GsonTarget gsonTarget = new GsonTarget();
        String templateString = "{\"hidden\": null, \"visible\": \"Visible\"}";

        byte[] result = gsonTarget.run(templateString.getBytes(StandardCharsets.UTF_8));
        System.out.println(new String(result, StandardCharsets.UTF_8));
    }
}
