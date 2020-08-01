import
  llvm_core, llvm_analysis, llvm_target

proc factorial =
  discard initializeNativeTarget()
  discard initializeNativeAsmPrinter()

  let module = moduleCreateWithName("fac_module")

  var facArgs = [int32Type()]
  let facType = functionType(int32Type(), facArgs[0].addr, 1, 0)
  let fac = module.addFunction("fac", facType)
  fac.setFunctionCallConv(CCallConv)
  let n = fac.getParam(0)

  let
    entry = fac.appendBasicBlock("entry")
    ifTrue = fac.appendBasicBlock("iftrue")
    ifFalse = fac.appendBasicBlock("iffalse")
    endBB = fac.appendBasicBlock("end")
    builder = createBuilder()

  builder.positionBuilderAtEnd(entry)
  let ifCmp = builder.buildICmp(IntEQ, n, constInt(int32Type(), 0, 0), "n == 0")
  discard builder.buildCondBr(ifCmp, ifTrue, ifFalse)

  builder.positionBuilderAtEnd(ifTrue)
  let resIfTrue = constInt(int32Type(), 1, 0)
  discard builder.buildBr(endBB)

  builder.positionBuilderAtEnd(ifFalse)
  var
    nMinus = builder.buildSub(n, constInt(int32Type(), 1, 0), "n - 1")
    callFacArgs = [nMinus]
    callFac = builder.buildCall(fac, callFacArgs[0].addr, 1, "fac(n - 1)")
    resIfFalse = builder.buildMul(n, callFac, "n * fac(n - 1)")
  discard builder.buildBr(endBB)

  builder.positionBuilderAtEnd(endBB)
  var
    res = builder.buildPhi(int32Type(), "result")
    phiVals = [resIfTrue, resIfFalse]
    phiBlocks = [ifTrue, ifFalse]
  res.addIncoming(phiVals[0].addr, phiBlocks[0].addr, 2)
  discard builder.buildRet(res)

  var error: cstring
  let errorP = cast[cstringArray](error.addr)

  discard verifyModule(module, AbortProcessAction, errorP)
  disposeMessage(error)




  module.dumpModule()


factorial()
