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
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/CFG.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/ADT/StringRef.h"
#include "stdio.h"
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
    
    map<BasicBlock*, set<BasicBlock*>> dominance;
    set<pair<BasicBlock*,BasicBlock*>> backedge;
    map<pair<BasicBlock*,BasicBlock*>,set<BasicBlock*>> blocknloop;
    int loop = 0;

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
        cerr<<"======================"<<endl;
        cerr<<"Number of loops: " << backedge.size()<<endl;
        cerr<<"Dominance relation: ";
        for(BasicBlock &BB : F){
                set<BasicBlock*> resDomset = dominance[&BB];
                for(BasicBlock* resDom : resDomset){
                    if(resDom->getName()!=BB.getName())
                        cerr<<ref2str(resDom->getName())<<" -> "<<ref2str(BB.getName())<<", ";
                }
                
            }
        cerr<<endl;
        cerr<<"Back edge: ";
        for( pair<BasicBlock*,BasicBlock*> pers : backedge){
            
            cerr<<ref2str(pers.first->getName())<<" -> "<<ref2str(pers.second->getName())<<", ";
        }
        cerr<<endl;
        int counter=0;
        for(pair<BasicBlock*,BasicBlock*> pers : backedge){
            counter++;
            cerr<<"Basic blocks in the loop "<<counter<<": ";
            for(BasicBlock* bbncir : blocknloop[pers]){
                cerr<<ref2str(bbncir->getName())<<", ";
            }
            cerr<<endl;
        }
        cerr<<"======================"<<endl;
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
    pair<string,string> arrayAccAna(Instruction* Inst){
        pair<string,string> res;
        res.first=Inst->getOperand(0)->getName();
        res.second=getIndexStr(Inst->getOperand(2));
        return res;
    }
    // NatLoop - Find all nature loops and do the dominator analysis.
    struct NatLoop : public FunctionPass {
        static char ID; // Pass identification, replacement for typeid
        NatLoop() : FunctionPass(ID) {}
        

                
        bool runOnFunction(Function &F) override {
            nameBasicBlocks(F);
            for(BasicBlock &BB : F){
                set<BasicBlock*> doms;
                for(BasicBlock &subBB : F){                
                    doms.insert(&subBB);
                }
                dominance[&BB]= doms;
            }
            bool flag=true;
            //BasicBlock entry = F.getEntryBlock();
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
            BasicBlock& BB = F.getEntryBlock();
            queue<BasicBlock*> nque;
            calallloops(BB,nque );
            set<Value*> basicIndTable= set <Value*>();
            set<Value*> derivedTable;
            for(pair<BasicBlock*,BasicBlock*> pers : backedge){
                /*basic indection variable*/ 
                set<Value*> storeTable = set<Value*>();
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
                
                /*errs()<<"Loop induction variable (derived): ";
                for(Value* Derived : derived){
                    errs()<<*Derived<<";";
                }
                errs()<<"\n";*/
                
            }
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
                    arrayAcc.insert(arrayAccAna(&*Inst));
                }
            }           
            for(pair<string,string> res : arrayAcc){
                errs()<<"Array access to "<<res.first<<" with index "<<res.second<<"\n";
            }
                
                
            return false;
        }
    };
}

char NatLoop::ID = 0;
static RegisterPass<NatLoop> X("nloop", "Nature Loop Analysis Pass");

