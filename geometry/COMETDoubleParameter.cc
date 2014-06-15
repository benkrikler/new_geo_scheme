#include "COMETDoubleParameter.hh"

#include <TFormula.h>
#include <ICOMETLog.hxx>
#include <sstream>

#define PrintValue(value) COMETError(#value<<"=|"<<value<<"|")

void COMETDoubleParameter::MakeValue(){
    // Set a value directly
    if(!fSetByExpression) return;

    // Get the values of all dependency parameters
    double vals[kMaxParams];
    IdParam tmp;
    for(DependencyList::const_iterator i_param=fParameters.begin();
            i_param!=fParameters.end(); i_param++){
        tmp=i_param->second;
        vals[tmp.id]=tmp.param->GetValue<double>();
    }
    // Set the value of all parameters in the formula
    fExpression->SetParameters(vals);
    // Evaluate this formula (the '0' is not used, but Eval has to have one
    // argument given)
    SetValue(fExpression->Eval(0));
}

COMETDoubleParameter::ParsingError COMETDoubleParameter::SetSource(const G4String& source){
    if(fExpression) delete fExpression;
    ParsingError ret_val=kSuccess;
    if (source[0]=='=') ret_val= SetByExpression(source.substr(1,std::string::npos));
    else ret_val= SetByValue(source);
    CheckError(source, ret_val);
    return ret_val;
}

COMETDoubleParameter::ParsingError COMETDoubleParameter::SetByValue(G4String expression){
    fSetByExpression=false;

    // Get the value and units from the string
    std::stringstream ss(expression);
    double val;
    G4String units,remainder;
    ss>>val>>units>>remainder;

    // There should be nothing left after the value
    if(!remainder.empty()) return kUnParseable;

    // Set the new values
    SetUnits(units);
    SetValue(val);
    return kSuccess;
}

COMETDoubleParameter::ParsingError COMETDoubleParameter::SetByExpression(G4String expression){
    fSetByExpression=true;

    // Strip out all the parameter names and replace with an ID
    size_t left_br=expression.index('[');
    size_t right_br=std::string::npos;
    int index;
    while(left_br!=std::string::npos){
        // Scan string for next right square bracket
        right_br=expression.index(']',left_br);
        if(right_br==std::string::npos) return kUnmatchedDelimiter;

        // Get the index for this dependency
        PrintValue(left_br);
        PrintValue(right_br);
        G4String parameter=expression(left_br+1,right_br-left_br-1);
        PrintValue(parameter);
        index= AddDependency(parameter);
        if(index<0) return kBadDependency;
        if(index>=kMaxParams) return kTooManyParams;
        PrintValue(index);

        // Replace parameter name with index
        expression.replace(left_br+1, right_br-left_br-1, Form("%d",index));

        // Scan string for next left square bracket
        left_br=expression.index('[',left_br+1);
    }

    // Create the new TFormula
    COMETError(expression);
    fExpression=new TFormula(GetName().c_str(),expression);

    // Success!
    return kSuccess;
}

G4String COMETDoubleParameter::GetDescription()const{
    if(fSetByExpression && fExpression) return fExpression->GetExpFormula().Data();
    return "";
}

int COMETDoubleParameter::AddDependency(const G4String& target){
    // Check if we already know of this parameter
    DependencyList::const_iterator it =fParameters.find(target);
    if(it!= fParameters.end()) return it->second.id;

    // Make a new parameter
    COMETExternalParameter<double>* param=COMETExternalParameter<double>::Make(GetController(),target);
    if(!param) return -1;

    // add the new parameter to the dependency lists
    fParametersIndices.push_back(target);
    IdParam step;
    step.id=fParametersIndices.size()-1;
    step.param=param;
    fParameters[target]=step;

    // return the position in the ordered list of dependencies
    return step.id;
}

void COMETDoubleParameter::CheckError(const G4String& input, ParsingError code){
    switch (code){
        case kUnmatchedDelimiter:
            COMETError("Unmatched delimiter: '[' in expression: "<<input);
            break;
        case kUnParseable:
            COMETError("Problem parsing expression "<<input);
            break;
        case kBadDependency:
            COMETError("Problem parsing a parameter in: "<<input);
            break;
        case kTooManyParams:
            COMETError("Too many different parameters in: "<<input);
            break;
        case kSuccess: break;
    }
}
