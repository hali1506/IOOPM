package org.ioopm.calculator.ast;

public class Clear extends Command
{
    private static final Clear theInstance = new Clear();

    private Clear() {}

    /**
     * Fetches the clear object
     * @return the clear object
     */
    public static Clear instance() {
        return theInstance;
    }

}