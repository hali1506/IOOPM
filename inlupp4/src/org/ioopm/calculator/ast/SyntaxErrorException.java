package org.ioopm.calculator.ast;

public class SyntaxErrorException extends RuntimeException{ //TODO, ska extenda IO
    public SyntaxErrorException(String msg){
        super(msg);
    }
}