//===- NatLoop.cpp - Code for Loop & DOM Analysis ------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the pass that can detects all BasicBlock within Loops
// And the DOM relation within these block.
//
//===----------------------------------------------------------------------===//

#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/CFG.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/ADT/StringRef.h"
#include "stdio.h"
#include <stdlib.h>
#include <string>
#include <iostream>
#include <map>
#include <set>
#include <list>
#include <queue>
#include <utility>



using namespace llvm;
using namespace std;


void nameBasicBlocks(Function &F);

namespace {
    
    // name each basic block in given function
    void nameBasicBlocks(Function &F) {	
        for (Function::iterator BB = F.begin(), FE = F.end(); BB != FE; ++BB) {
		    if (!BB->hasName()) {
                BB->setName("BB_");
            }
        }
    }
    /*struct Node{
        BasicBlock* bb;
        Node** next;
    };
    struct Path{
        Node* begin;
        Node* end;
        Node* cur;
    };*/
    struct ArrIdxTuple{
	    string name;
	    int a;
	    int b;
    };

    map<BasicBlock*, set<BasicBlock*>> dominance;
    set<pair<BasicBlock*,BasicBlock*>> backedge;
    map<pair<BasicBlock*,BasicBlock*>,set<BasicBlock*>> blocknloop;
    map<Instruction*,ArrIdxTuple> referenceTable;
    map<pair<pair<Instruction*, string>,pair<Instruction*, string>>,pair<vector<int>,vector<string>>> map2vector;

    int loop = 0;
    
    bool equals(ArrIdxTuple a,ArrIdxTuple b){
	    if(a.name!=b.name)
		    return false;
	    if(a.a!=b.a)
		    return false;
	    if(a.b!=b.b)
		    return false;
	    return true;
    }
    
    string numberDic = "0123456789";
    string elimnum(string raw){
        size_t pos = raw.length();
        for(size_t iter = 0; iter<raw.length(); iter++){
            if(numberDic.find(raw[iter])== string::npos){
                pos = iter;
            }
        }
        return raw.substr(0,pos+1);
    }
    string ref2str(StringRef Ref){
        string s; 
        for (StringRef::iterator c = Ref.begin(); c != Ref.end(); c++){
            s += *c;
        }
        return s;
    }
    
    void calallloops(BasicBlock& BB, queue<BasicBlock*> loopline){
        loopline.push(&BB);
        for(BasicBlock* succ : successors(&BB)){
            queue<BasicBlock*> localline= loopline;
            set<BasicBlock*>::iterator loc = dominance[&BB].find(succ);
                    if(loc != dominance[&BB].end()){
                        loop++;
                        
                        pair<BasicBlock*,BasicBlock*> idx (&BB,*loc);
                        backedge.insert(idx);
                        while(localline.front()!=(BasicBlock*)*loc&&!localline.empty()){
                            localline.pop();
                        }
                        if(blocknloop[idx].empty()){             
                            set<BasicBlock*> l;
                            blocknloop[idx]= l;
                        }
                        while(!localline.empty()){
                            blocknloop[idx].insert(localline.front());
                            localline.pop();
                        }
                    }else{
                        calallloops(*succ, localline);
                    }
                        
        }
    }
    
    void dum(Function& F){
        errs()<<"======================"<<"\n";
        errs()<<"Number of loops: " << backedge.size()<<"\n";
        errs()<<"Dominance relation: ";
        for(BasicBlock &BB : F){
                set<BasicBlock*> resDomset = dominance[&BB];
                for(BasicBlock* resDom : resDomset){
                    if(resDom->getName()!=BB.getName())
                        errs()<<ref2str(resDom->getName())<<" -> "<<ref2str(BB.getName())<<", ";
                }
                
            }
        errs()<<"\n";
        errs()<<"Back edge: ";
        for( pair<BasicBlock*,BasicBlock*> pers : backedge){
            
            errs()<<ref2str(pers.first->getName())<<" -> "<<ref2str(pers.second->getName())<<", ";
        }
        errs()<<"\n";
        int counter=0;
        for(pair<BasicBlock*,BasicBlock*> pers : backedge){
            counter++;
            errs()<<"Basic blocks in the loop "<<counter<<": ";
            for(BasicBlock* bbncir : blocknloop[pers]){
                errs()<<ref2str(bbncir->getName())<<", ";
            }
            errs()<<"\n";
        }
        errs()<<"======================"<<"\n";
    }

    bool inLoop(Instruction* V, set<BasicBlock*> loop){
        for(BasicBlock* BB : loop){
            if(V->getParent()==BB){
                return true;
            }
        }
        return false;
    }
    bool isLoopInvariant(Value* val, set<BasicBlock*> loop){
        if(isa<Constant>(val)||isa<Argument>(val)||isa<GlobalValue>(val)){
            return true;
        
        }
        if(val==nullptr)
            return false;
        return !inLoop(dyn_cast<Instruction> (val), loop);
    }
    /*string ref2str(StringRef ref){
        string str;
        for(char* i:ref){
            str.append(*i);
        }
        return str;
    }*/
    string getIndexStr(Value* V){
        
        if(isa<ConstantInt>(V)){
            return to_string(dyn_cast<ConstantInt>(V)->getSExtValue());
        }
       // if(isa<ConstantFP>(V)){
       //     return to_string(dyn_cast<ConstantFP>(V)->getValue());
       // }
        if(isa<LoadInst>(V)){
            return ref2str(dyn_cast<Instruction>(V)->getOperand(0)->getName());
        }
        if(isa<Instruction>(V)){
            Instruction* Inst = dyn_cast<Instruction> (V);
            if(isa<SExtInst>(Inst)){
                return getIndexStr(Inst->getOperand(0));
            }
            string res="";
            if(Inst->getOpcode()==Instruction::Add){
                res.append(getIndexStr(Inst->getOperand(0)));
                res.append(" + ");
                res.append(getIndexStr(Inst->getOperand(1)));
                
            }
            if(Inst->getOpcode()==Instruction::Sub){
                res.append(getIndexStr(Inst->getOperand(0)));
                res.append(" - ");
                res.append(getIndexStr(Inst->getOperand(1)));
            }
            if(Inst->getOpcode()==Instruction::Mul){                                          
                res.append(getIndexStr(Inst->getOperand(0)));
                res.append(" * ");
                res.append(getIndexStr(Inst->getOperand(1)));
            }
            return res;
        }
        return ref2str(V->getName());
    }

    int getIndexTuple(Value* V, ArrIdxTuple& Idx){
        
        if(isa<ConstantInt>(V)){
            return (int)dyn_cast<ConstantInt>(V)->getSExtValue();
        }
       // if(isa<ConstantFP>(V)){
       //     return to_string(dyn_cast<ConstantFP>(V)->getValue());
       // }
        if(isa<LoadInst>(V)){
            Idx.name=ref2str(dyn_cast<Instruction>(V)->getOperand(0)->getName());
            Idx.a=1;
	        return 0;
        }
        if(isa<Instruction>(V)){
            Instruction* Inst = dyn_cast<Instruction> (V);
            if(isa<SExtInst>(Inst)){
                return getIndexTuple(Inst->getOperand(0),Idx);
            }
            
            if(Inst->getOpcode()==Instruction::Add){
                int flag=getIndexTuple(Inst->getOperand(0),Idx);
		        if(flag!=0){
        	        Idx.b=flag;
	            }
                flag = getIndexTuple(Inst->getOperand(1),Idx);
		        if(flag!=0){
			        Idx.b=flag;
			    }
		        return 0;
            }
            if(Inst->getOpcode()==Instruction::Sub){
                int flag=getIndexTuple(Inst->getOperand(0),Idx);
		        if(flag!=0){
		        	Idx.b=flag;
		        }
                flag = getIndexTuple(Inst->getOperand(1),Idx);
		        if(flag!=0){
		        	Idx.b=-flag;
			    }else{
		        	Idx.a*=-1;
		        }
		        return 0;
            }
            if(Inst->getOpcode()==Instruction::Mul){                                          
                int flag=getIndexTuple(Inst->getOperand(0),Idx);
		        if(flag!=0){
		        	Idx.a=flag;
		        }
                flag = getIndexTuple(Inst->getOperand(1),Idx);
		        if(flag!=0){
		        	Idx.a=flag;
		        }
		        return 0;
            }
            
        }
        errs()<<"Tuple analysis fails"<<"\n";
	    return 0;
    }

    int nameCounter = 0;
    pair<Instruction*,string> arrayAccAna(Instruction* Inst){
        ArrIdxTuple index;
	    pair<Instruction*,string> res;
	    string prename = ref2str(Inst->getOperand(0)->getName());
        string oper="";
        index.a=index.b=0;
        int theb = getIndexTuple(Inst->getOperand(Inst->getNumOperands()-1),index);
        if(theb!=0)
        index.b=theb;
	    for(User* u: Inst->users()){
		    if(isa<StoreInst>(u)){
			    oper="store";
		    }
		    if(isa<LoadInst>(u)){
			    oper="load";
		    }
	    }
	/*for (pair<string,ArrIdxTuple> ref : referenceTable){
		if(equals(ref.second,index)){
			res.first=ref.first;
			res.second=oper;
			Inst->setName(res.first);
			return res;
		}
	}*/
	    prename+=to_string(nameCounter);
        nameCounter++;
	    
	    
	    res.second=oper;
	    
        if(oper=="store"){
            Inst->setName(prename);
            res.first=Inst;

        }
	    if(oper=="load"){
            for(User* load : Inst->users()){
                if(isa<LoadInst>(load)){
                    res.first=dyn_cast<Instruction>(load);
                    res.first->setName(prename);
                }
            }
        }
        referenceTable[res.first]=index;
        return res;
    }

    pair<string,string> arrayAccAna(Instruction* Inst,bool t){
        pair<string,string> res;
        res.first=Inst->getOperand(0)->getName();
        res.second=getIndexStr(Inst->getOperand(2));
        return res;
    }

    void dominanceAnaly(Function& F){
	bool flag = true;
    	while(flag){
                //set<BaisicBlock*> visitedbb;
                flag=false;
                for(BasicBlock &BB : F){
                    set<BasicBlock*> MODom;
                    int firstpred=1;
                    for (BasicBlock* pred : predecessors(&BB)){
                        if (firstpred==1){
                            MODom=dominance[pred];
                            firstpred=0;
                        }
                        else{
                            set<BasicBlock*> removeDom;
                            for(BasicBlock* uncertendom : MODom){
                                if(dominance[pred].find(uncertendom)==dominance[pred].end()){
                                    removeDom.insert(uncertendom);
                                }
                            }
                            for(BasicBlock* uncertendom : removeDom){
                                MODom.erase(uncertendom);
                            }
                        }
                    }
                    MODom.insert(&BB);
                    if(MODom.size()!=dominance[&BB].size()){
                        flag=true;
                    }else{
                        for(BasicBlock* certenDom : MODom){
                            if(dominance[&BB].find(certenDom)==dominance[&BB].end()){
                                flag=true;
                            }
                        }
                    }
                    dominance[&BB]=MODom;
                }
            }

    }
    
    void findBasic(set<Value*> & basicIndTable,set<Value*>& storeTable,pair<BasicBlock*,BasicBlock*> pers){
    	for(BasicBlock* bbncir : blocknloop[pers]){
                    for(Instruction &I : *bbncir){
                        if(isa<StoreInst>(I)){
                            User::op_iterator u = I.op_begin();
                            u++;        
                            Value* v = u->get();
                            storeTable.insert(v);
                        }
                    }
                    set<Value*>& newStoreTable=basicIndTable;
                    for(Value* v : storeTable){
                        for(User *u : v->users()){
                            Instruction *Inst = dyn_cast<Instruction>(u);
                            if(!inLoop(Inst,blocknloop[pers])){
                                continue;
                            }
                            if(isa<StoreInst>(Inst)){
                                if(isa<Constant>(Inst->op_begin())){
                                    break;
                                }
                                Instruction* temppara= dyn_cast<Instruction>(Inst->op_begin());

                                Value* add_var = nullptr;
                                if(temppara->getOpcode()==Instruction::Add){
                                    if(isLoopInvariant(temppara->getOperand(0),blocknloop[pers])){
                                        add_var = temppara->getOperand(1);
                                    }else if(isLoopInvariant(temppara->getOperand(1),blocknloop[pers])){
                                        add_var = temppara->getOperand(0);
                                    }else{
                                        break;
                                    }
                                }else if(temppara->getOpcode()==Instruction::Sub){
                                    if(isLoopInvariant(temppara->getOperand(1),blocknloop[pers])){
                                        add_var = temppara->getOperand(0);
                                    }else{
                                        break;
                                    }
                                }else{
                                    break;
                                }
                                if(add_var==nullptr){
                                    errs()<<"null pointer detected!"<<"\n";
                                }
                                Instruction *load_inst = dyn_cast<Instruction>(add_var);
                                if(isa<LoadInst>(load_inst)){
                                    if(load_inst->getOperand(0)==Inst->getOperand(1)){
                                        newStoreTable.insert(load_inst->getOperand(0));
                                        break;
                                    }
                                    break;
                                }
                                break;
                            }
			}
                    }
                    //for(Value* v: newStoreTable ){
                    //    errs()<<*v<<"\n";
                    //}
                }

    }
    void findDerived(set<Value*> & derived,set<User*>& loadTable,pair<BasicBlock*,BasicBlock*> pers){
    	for(User* U : loadTable){
                    //Iterate over all multiplication instructions
                    for(User* mul : U->users()){
                        Instruction* mul_inst = dyn_cast<Instruction> (mul);
                        if(mul_inst->getOpcode()==Instruction::Mul){
                            //Iterates over all add or sub instructions
                            for( User* add : mul_inst->users() ){
                                Instruction* add_inst = dyn_cast<Instruction> (add);
                                Instruction* store_inst = nullptr;
                                if(isa<StoreInst>(add_inst)){
                                    if(add_inst->getOperand(0)==dyn_cast<Value>(mul)&&add_inst->getOperand(1)!=U->getOperand(0)){
                                        derived.insert(add_inst->getOperand(1));
                                    }
                                }else if(add_inst->getOpcode()==Instruction::Add||add_inst->getOpcode()==Instruction::Sub){
                                    if(isLoopInvariant(add_inst->getOperand(0),blocknloop[pers])||isLoopInvariant(add_inst->getOperand(1),blocknloop[pers])){
                                        //Iterate over all store instructions
                                        for(User* store : add_inst->users()){
                                            store_inst = dyn_cast<Instruction>(store);
                                            if(isa<StoreInst>(store_inst)){
                                                if(store_inst->getOperand(0)==dyn_cast<Value>(add_inst)&&store_inst->getOperand(1)!=U->getOperand(0)){
                                                    derived.insert(store_inst->getOperand(1));
                                                }else{
                                                    break;
                                                }     
                                            }
                                        }
                                    }else{
                                        continue;
                                    }
                            
                                }
                            }
                        }else {
                            Instruction* add_inst = mul_inst;
                            Instruction* store_inst = nullptr;
                            if(isa<StoreInst>(add_inst)){
                                if(add_inst->getOperand(0)==dyn_cast<Value>(mul)&&add_inst->getOperand(1)!=U->getOperand(0)){
                                    derived.insert(add_inst->getOperand(1));
                                }
                            }else if(add_inst->getOpcode()==Instruction::Add||add_inst->getOpcode()==Instruction::Sub){
                                if(isLoopInvariant(add_inst->getOperand(0),blocknloop[pers])||isLoopInvariant(add_inst->getOperand(1),blocknloop[pers])){
                                    //Iterate over all store instructions                           
                                    for(User* store : add_inst->users()){                           
                                        store_inst = dyn_cast<Instruction>(store);     
                                        if(isa<StoreInst>(store_inst)){                             
                                            if(store_inst->getOperand(0)==dyn_cast<Value>(add_inst)&&store_inst->getOperand(1)!=U->getOperand(0)){
                                                derived.insert(store_inst->getOperand(1));          
                                            }else{                                                  
                                                break;                                              
                                            }                                                       
                                        }                                                           
                                    }                                        
                                }else{                                       
                                    continue;                                
                                }
                            }                                
                        }
                    }
                }

    }

    string ZIV(pair<pair<Instruction*, string>,pair<Instruction*, string>> refPair,int beta1,int beta2){
        string res="Applied ZIV.\n";
        if(beta1!=beta2){
            res+="There is no dependence between them.\n";
            return res;
        }
        res+="There is an ";
        if(refPair.first.second=="load"&&refPair.second.second=="store"){
            res+="anti-dependence from "+ref2str(refPair.second.first->getName())+" on "+ref2str(refPair.first.first->getName())+".\n";
        }else if(refPair.second.second=="store"){
            res+="output dependence from "+ref2str(refPair.second.first->getName())+" on "+ref2str(refPair.first.first->getName())+".\n";
        }else{
            res+="true dependence from "+ref2str(refPair.second.first->getName())+" on "+ref2str(refPair.first.first->getName())+".\n";
        }
        res+="It is a loop independent dependence.\n";    
        return res;
    }

    string WZSIV(pair<pair<Instruction*, string>,pair<Instruction*, string>> refPair,int alpha,int beta1,int beta2,int i1,int i2){
        string res="Applied WZSIV.\n";
        /*if(i2<=abs(beta2-beta1)/alpha||i1>abs(beta2-beta1)){

            return res;
        }*/
        int equalFlag=0;
        int interval=abs(i2-i1);
        for(int dir : map2vector[refPair].first){
            if(dir==0){
                equalFlag=1;
            }
            if(dir>0&&dir<=interval){
                equalFlag=2;
                break;
            }
        }
        if(equalFlag ==0){
            res+="There is no dependence between them.\n";
            return res;
        }
        res+="There is a";
        if(refPair.first.second=="load"&&refPair.second.second=="store"){
            res+="n anti-dependence from "+ref2str(refPair.second.first->getName())+" on "+ref2str(refPair.first.first->getName())+".\n";
        }else if(refPair.second.second=="store"){
            res+=" output dependence from "+ref2str(refPair.second.first->getName())+" on "+ref2str(refPair.first.first->getName())+".\n";
        }else{
            res+=" true dependence from "+ref2str(refPair.second.first->getName())+" on "+ref2str(refPair.first.first->getName())+".\n";
        }
        if(equalFlag == 1){
            res+="It is a loop independent dependence.\n";
        }else{
            res+="It is a loop-carried dependence.\n";
        }
        return res;
    }

    string SSIV(pair<pair<Instruction*, string>,pair<Instruction*, string>> refPair,int alpha,int beta1,int beta2,int i1,int i2){
        string res="Applied SSIV.\n";
        /*if(dis<abs(beta2-beta1)/alpha){
            res+="There is no dependence between them.\n";
            return res;
        }*/
        int equalFlag=0;
        int interval=abs(i2-i1);
        for(int dir : map2vector[refPair].first){
            if(dir==0){
                equalFlag=1;
            }
            if(dir>0&&dir<=interval){
                equalFlag=2;
                break;
            }
        }
        if(equalFlag ==0){
            res+="There is no dependence between them.\n";
            return res;
        }
        res+="There is a";
        if(refPair.first.second=="load"&&refPair.second.second=="store"){
            res+="n anti-dependence from "+ref2str(refPair.second.first->getName())+" on "+ref2str(refPair.first.first->getName())+".\n";
        }else if(refPair.second.second=="store"){
            res+=" output dependence from "+ref2str(refPair.second.first->getName())+" on "+ref2str(refPair.first.first->getName())+".\n";
        }else{
            res+=" true dependence from "+ref2str(refPair.second.first->getName())+" on "+ref2str(refPair.first.first->getName())+".\n";
        }
        if(equalFlag == 1){
            res+="It is a loop independent dependence.\n";
        }else{
            res+="It is a loop-carried dependence.\n";
        }
        return res;
    }

    string WCSIV(pair<pair<Instruction*, string>,pair<Instruction*, string>> refPair,int alpha,int beta1,int beta2,int i1,int i2){
        string res="Applied WCSIV.\n";
        /*if(i2<=(beta2-beta1)/alpha/2||i1>(beta2-beta1)/alpha/2){
            res+="There is no dependence between them.\n";
            return res;
        }*/
        int equalFlag=0;
        int interval=abs(i2-i1);
        /*for(int dir : map2vector[refPair].first){
            if(dir==0){
                equalFlag=1;
            }
            if(dir>0&&dir<=interval&&equalFlag==1){
                equalFlag=2;
                break;
            }
        }*/
	int mean = (beta1+beta2)/2;
	if(i1 == mean|| i2 == mean)
	equalFlag = 1;
	if(i1>mean&&i2<mean)
	equalFlag = 2;
	if(i2 == mean && mean*2 < beta1 + beta2)
	equalFlag = 2;
        if(equalFlag ==0){
            res+="There is no dependence between them.\n";
            return res;
        }
        res+="There is a";
        if(refPair.first.second=="load"&&refPair.second.second=="store"){
            res+="n anti-dependence from "+ref2str(refPair.second.first->getName())+" on "+ref2str(refPair.first.first->getName())+".\n";
        }else if(refPair.second.second=="store"){
            res+=" output dependence from "+ref2str(refPair.second.first->getName())+" on "+ref2str(refPair.first.first->getName())+".\n";
        }else{
            res+=" true dependence from "+ref2str(refPair.second.first->getName())+" on "+ref2str(refPair.first.first->getName())+".\n";
        }
        if(equalFlag == 1){
            res+="It is a loop independent dependence.\n";
        }else{
            res+="It is a loop-carried dependence.\n";
        }
        return res;
    }

    // NatLoop - Find all nature loops and do the dominator analysis.
    struct InstCounter : public FunctionPass {
        static char ID; // Pass identification, replacement for typeid
        InstCounter() : FunctionPass(ID) {}
        

                
        bool runOnFunction(Function &F) override {
            nameBasicBlocks(F);
            for(BasicBlock &BB : F){
                set<BasicBlock*> doms;
                for(BasicBlock &subBB : F){                
                    doms.insert(&subBB);
                }
                dominance[&BB]= doms;
            }
            dominanceAnaly(F);
	        BasicBlock& BB = F.getEntryBlock();
            queue<BasicBlock*> nque;
            calallloops(BB,nque );
            set<Value*> basicIndTable= set <Value*>();
            set<Value*> derivedTable;
            for(pair<BasicBlock*,BasicBlock*> pers : backedge){
                /*basic indection variable*/ 
                set<Value*> storeTable = set<Value*>();
		        findBasic(basicIndTable,storeTable,pers);
                /*derivable induction variable*/
                set<User*> loadTable= set<User*>();
                for(Value* V : basicIndTable){
                    for(User* U : V->users()){
                        if(isa<LoadInst>(U)&&inLoop(dyn_cast<Instruction>(U),blocknloop[pers])){
                            loadTable.insert(U);
                        }    
                    } 
                }
                set<Value*>& derived = derivedTable;
                //Iterate over all load instructions
                findDerived(derived,loadTable,pers); 

                
            }

            set<pair<Instruction*,string>> loadReference;
            set<pair<Instruction*,string>> storeReference;
            set<pair<pair<Instruction*,string>,pair<Instruction*,string>>> referencePairs;
            //look for lower bound and upperbound
            int lowbound,upbound;
            Value* loopvar = *(basicIndTable.begin());
            for(User* store : loopvar->users()){
                if(isa<StoreInst>(store)){
                    StoreInst* storeInst=dyn_cast<StoreInst>(store);
                    for(pair<BasicBlock*,BasicBlock*> pers : backedge){
                        if(!inLoop(storeInst,blocknloop[pers])){
                            if(isa<Constant>(storeInst->getOperand(0))){
                                lowbound=dyn_cast<ConstantInt>(storeInst->getOperand(0))->getSExtValue();
                            }
                        }
                    }        
                }
            }
            for(inst_iterator Inst = inst_begin(F), E = inst_end(F); Inst != E; ++Inst){
                if(isa<GetElementPtrInst>(&*Inst)){
			        pair<Instruction*,string> ref=arrayAccAna(&*Inst);
                    if(ref.second=="store"){
                        storeReference.insert(ref);
                    }else if(ref.second=="load"){
                        loadReference.insert(ref);
                    }
                }
		        if(isa<BranchInst>(&*Inst)){
                    BranchInst* br=dyn_cast<BranchInst>(&*Inst);
                    if(br->isConditional())
                        upbound=dyn_cast<ConstantInt>(dyn_cast<Instruction>(br->getCondition())->getOperand(1))->getSExtValue();
                }
            }           

            for(pair<Instruction*,string> ref: storeReference){
                for(pair<Instruction*, string> store : storeReference){
                    if(ref2str(ref.first->getName())[0]==ref2str(store.first->getName())[0]&&ref.first->getName()!=store.first->getName())
                        referencePairs.insert(pair<pair<Instruction*, string>,pair<Instruction*, string>>(ref,store));
                }
                for(pair<Instruction*, string> load : loadReference){
                    if(ref2str(ref.first->getName())[0]==ref2str(load.first->getName())[0]){
                        referencePairs.insert(pair<pair<Instruction*, string>,pair<Instruction*, string>>(ref,load));
                        referencePairs.insert(pair<pair<Instruction*, string>,pair<Instruction*, string>>(load,ref));
                    }
                }
            }
            
            for(pair<pair<Instruction*, string>,pair<Instruction*, string>> referPair: referencePairs){
                vector<int> distance;
                vector<string> direction;
                if(referenceTable[referPair.first.first].a==referenceTable[referPair.second.first].a){
		            distance.push_back(referenceTable[referPair.first.first].b-referenceTable[referPair.second.first].b);
                    //distance.push_back(referenceTable[referPair.first.first].a);
                    //distance.push_back(referenceTable[referPair.first.first].b);
                    //distance.push_back(referenceTable[referPair.second.first].b);
                    if(distance[0]>0){
                        direction.push_back("<");
                    }else if(distance[0]<0){
                        direction.push_back(">");
                    }else{
                        direction.push_back("=");
                    }
                }else{
                    for(int i = lowbound; i<upbound; i++){
                        distance.push_back(i*(referenceTable[referPair.first.first].a-referenceTable[referPair.second.first].a)+referenceTable[referPair.first.first].b-referenceTable[referPair.second.first].b);
                        //distance.push_back(referenceTable[referPair.first.first].a);
                        //distance.push_back(referenceTable[referPair.first.first].b);
                        //distance.push_back(referenceTable[referPair.second.first].b);
                        if(distance[i-lowbound]>0){
                            direction.push_back("<");
                        }else if(distance[i-lowbound]<0){
                            direction.push_back(">");
                        }else{
                            direction.push_back("=");
                        }
                    }
                }
                map2vector[referPair]=pair<vector<int>,vector<string>>(distance,direction);
            }
            
            //dependence testing
            map<pair<pair<Instruction*, string>,pair<Instruction*, string>>,string> dependenceRes;
            for(pair<pair<Instruction*, string>,pair<Instruction*, string>> referPair: referencePairs){
                string res="";
                if(referenceTable[referPair.first.first].a==0&&referenceTable[referPair.second.first].a==0){
                        res=ZIV(referPair,referenceTable[referPair.first.first].b,referenceTable[referPair.second.first].b);
                }else if(referenceTable[referPair.first.first].a==0||referenceTable[referPair.second.first].a==0){
                        res=WZSIV(referPair,referenceTable[referPair.first.first].a+referenceTable[referPair.second.first].a,referenceTable[referPair.first.first].b,referenceTable[referPair.second.first].b,upbound,lowbound);
                }else if(referenceTable[referPair.first.first].a*referenceTable[referPair.second.first].a>0){
                        res=SSIV(referPair,referenceTable[referPair.first.first].a,referenceTable[referPair.first.first].b,referenceTable[referPair.second.first].b,upbound,lowbound);
                }else if(referenceTable[referPair.first.first].a*referenceTable[referPair.second.first].a<0){
                        res=WCSIV(referPair,abs(referenceTable[referPair.first.first].a),referenceTable[referPair.first.first].b,referenceTable[referPair.second.first].b,upbound,lowbound);
                }else{
                    res="test fail.\n";
                }
                dependenceRes[referPair]=res;
            }

	    
            //information dump
	    dum(F);
	    //dump the Induction variable info
	    errs()<<"Loop induction variable (basic): ";
            if(basicIndTable.empty()){
                errs()<<"None.\n";
            }else{
                for(Value* basic : basicIndTable){
                    errs()<<"  "<<ref2str(basic->getName());
                }
                errs()<<"\n";
            }
            errs()<<"Loop induction variable (derived): ";
            if(basicIndTable.empty()){
                errs()<<"None.\n";
            }else{
                for(Value* Derived : derivedTable){
                    errs()<<ref2str(Derived->getName());
                }
                errs()<<"\n";
            }
	    set<pair<string,string>> arrayAcc;
            for(inst_iterator Inst = inst_begin(F), E = inst_end(F); Inst != E; ++Inst){
                if(isa<GetElementPtrInst>(&*Inst)){
                    arrayAcc.insert(arrayAccAna(&*Inst,true));
                }
            }  
	    for(pair<string,string> res : arrayAcc){
                errs()<<"Array access to "<<res.first<<" with index "<<res.second<<"\n";
            }

	    errs()<<"======================"<<"\n";

	    //dump the dependence testing info
            errs()<<"ID assigned & operation: ";

            for(pair<Instruction*,string> ref: loadReference){
                
                errs()<<" <"<<ref2str(dyn_cast<Value>(ref.first)->getName())<<", "<<ref.second<<">,";

            }
            for(pair<Instruction*,string> ref: storeReference){
                errs()<<" <"<<ref2str(ref.first->getName())<<", "<<ref.second<<">,";
            }
            errs()<<";\nReference pairs in loop i:\n";
            for(pair<pair<Instruction*, string>,pair<Instruction*, string>> refp: referencePairs){
                errs()<<"("<<refp.first.second<<" "<<ref2str(refp.first.first->getName())<<", "<<refp.second.second<<" "<<ref2str(refp.second.first->getName())<<"), ";
            }
            errs()<<"\n";
            for(pair<pair<Instruction*, string>,pair<Instruction*, string>> referPair: referencePairs){
                errs()<<"("<<ref2str(referPair.first.first->getName())<<", "<<ref2str(referPair.second.first->getName())<<")";
                for(int i : map2vector[referPair].first){
                    errs()<<" "<<to_string(i);
                }
                for(string i : map2vector[referPair].second){
                    errs()<<" "<<i;
                }
                errs()<<"\n";
            }
            errs()<<"Dependence Testing:\n";
            for(pair<pair<Instruction*, string>,pair<Instruction*, string>> referPair: referencePairs){
                errs()<<"("<<ref2str(referPair.first.first->getName())<<", "<<ref2str(referPair.second.first->getName())<<"):\n";
                errs()<<dependenceRes[referPair];
                errs()<<"\n";
            }
	    errs()<<"======================"<<"\n";

	    int count = 0;
	    for(pair<BasicBlock*,BasicBlock*> loop : backedge){
		    count++;
		    unsigned start = 0;
		    unsigned end = 0;
		    for(BasicBlock* bsblock : blocknloop[loop]){
			    for(BasicBlock::iterator II = bsblock->begin(); II != bsblock->end(); ++II){
				    if(DILocation *Loc = II->getDebugLoc()){
					    unsigned line = Loc->getLine();
                        if(line>end)
                        end = line;
					    if(start == 0)
						start = end;
				    }
			    }
		    }
		    errs()<<"loop"<<to_string(count)<<" (line"<<start<<"-"<<end<<"): ";
		    bool carried = false;
		    string acpair = "";
		    for(pair<pair<pair<Instruction*, string>,pair<Instruction*, string>>,string> referPair : dependenceRes){
			    if(inLoop(referPair.first.first.first,blocknloop[loop])&&inLoop(referPair.first.second.first,blocknloop[loop])){
				    if(referPair.second.find("loop-carried")!=string::npos){
					    carried = true;
					    string pairInfo = "(";
					    string v1Name = elimnum(ref2str(referPair.first.first.first->getName()));

					    pairInfo += v1Name + "[";
					    if(referenceTable[referPair.first.first.first].a == -1)
					    	    pairInfo += "-";
                        if(referenceTable[referPair.first.first.first].a != 0){
					        pairInfo +=elimnum(referenceTable[referPair.first.first.first].name);
                            if(referenceTable[referPair.first.first.first].b > 0)
                                pairInfo+="+";
                            if(referenceTable[referPair.first.first.first].b != 0)
                                pairInfo+=to_string(referenceTable[referPair.first.first.first].b);
                        }else{
                            pairInfo += to_string(referenceTable[referPair.first.first.first].b);
                        }
                        pairInfo += "], ";
					    string v2Name = elimnum(ref2str(referPair.first.second.first->getName()));

					    pairInfo += v2Name + "[";

					    if(referenceTable[referPair.first.second.first].a == -1)
					    	    pairInfo += "-";
					    if(referenceTable[referPair.first.second.first].a != 0){
					        pairInfo +=elimnum(referenceTable[referPair.first.second.first].name);
                            if(referenceTable[referPair.first.second.first].b > 0)
                                pairInfo+="+";
                            if(referenceTable[referPair.first.second.first].b != 0)
                                pairInfo+=to_string(referenceTable[referPair.first.second.first].b);
                        }else{
                            pairInfo += to_string(referenceTable[referPair.first.second.first].b);
                        }
                        pairInfo += "])";
					    acpair+=pairInfo;

				    }
			    }
		    }
		    if(carried){
			    errs()<<"Not parallelizable, loop carried dependence for array access pair "<<acpair<<"\n";
		    }else{
			    errs()<<"\"#progma omp parrallel for\"\n";
		    }
	    }
            return false;
        }
    };
}

char InstCounter::ID = 0;
static RegisterPass<InstCounter> X("nloop", "Nature Loop Analysis Pass");

