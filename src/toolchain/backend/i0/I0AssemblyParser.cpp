#include "I0AssemblyParser.h"

#include <core/Target/TargetProgram.h>
#include <antlr3.h>
#include <assert.h>
#include <I0Parser.h>
#include <I0Lexer.h>

void I0AssemblyParser::Parse(std::string fileName)
{
    Parse(fileName, SymbolScope::GetRootScope());
}

void I0AssemblyParser::Parse(std::string fileName, SymbolScope *scope)
{
    
    CompilationContext *context = CompilationContext::GetInstance();

    pANTLR3_INPUT_STREAM  input  = antlr3FileStreamNew((pANTLR3_UINT8)(fileName.c_str()), ANTLR3_ENC_UTF8);
   
    pI0Lexer  lxr  = I0LexerNew(input);      // CLexerNew is generated by ANTLR
    assert(lxr != NULL);

    pANTLR3_COMMON_TOKEN_STREAM  tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lxr));
    assert (tstream != NULL);

    pI0Parser psr = I0ParserNew(tstream);
    assert (psr != NULL);
    
    CompilationContext::GetInstance()->CurrentParser = psr->pParser;
    
    std::vector<TargetInstruction *> *code = psr->translation_unit(psr, scope);
    
    CompilationContext::GetInstance()->CurrentParser = NULL;   
    
    //TODO: Multiple source files
    context->Target = new TargetProgram();
    context->Target->Code = *code;
}

I0AssemblyParser::I0AssemblyParser()
{

}

I0AssemblyParser::~I0AssemblyParser()
{

}
