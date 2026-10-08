package org.ioopm.calculator.ast;

public class IllegalExpressionException extends RuntimeException{
    public IllegalExpressionException(String msg){
        super(msg);
    }
    public IllegalExpressionException() {}
}