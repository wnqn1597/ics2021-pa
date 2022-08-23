def_EHelper(lw) {
  rtl_lm(s, ddest, dsrc1, id_src2->imm, 4);
}

def_EHelper(sw) {
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 4);
}

def_EHelper(jal) {
  //rtl_addi(s, &(s->dnpc), &(s->pc), id_src1->imm);
  rtl_j(s, s->pc + id_src1->imm);
  rtl_mv(s, ddest, &(s->snpc));
}

def_EHelper(jalr) {
  rtl_addi(s, &(s->dnpc), dsrc1, id_src2->imm);
  rtl_mv(s, ddest, &(s->snpc));
}
