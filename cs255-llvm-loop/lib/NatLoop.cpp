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
#include "llvm/IR/Constants.h"
#include "llvm/IR/CFG.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/CFG.h"
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
            dum(F);       
                            
             
                
                
            return false;
        }
    };
}

char NatLoop::ID = 0;
static RegisterPass<NatLoop> X("nloop", "Nature Loop Analysis Pass");

