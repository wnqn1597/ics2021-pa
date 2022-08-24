def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm+1);
}

def_EHelper(auipc) {
  rtl_addi(s, ddest, &(s->pc), id_src1->imm);
}

def_EHelper(addi) {
  rtl_addi(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(slti) {
  
  *ddest = ((int32_t)*dsrc1 < (int32_t)id_src2->imm) ? 1 : 0;
}

def_EHelper(sltiu) {
  *ddest = (*dsrc1 < id_src2->imm) ? 1 : 0;
}

def_EHelper(xori) {
  rtl_xori(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(ori) {
  rtl_ori(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(andi) {
  rtl_andi(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(slli) {
  rtl_slli(s, ddest, dsrc1, (id_src2->imm)%32);
}

def_EHelper(srli) {
  rtl_srli(s, ddest, dsrc1, (id_src2->imm)%32);
}

def_EHelper(srai) {
  rtl_srai(s, ddest, dsrc1, (id_src2->imm)%32);
}

def_EHelper(add) {
  rtl_add(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sub) {
  rtl_sub(s, ddest, dsrc1, dsrc2);
}

def_EHelper(slt) {
  *ddest = ((int32_t)*dsrc1 < (int32_t)*dsrc2) ? 1 : 0;
}

def_EHelper(sltu) {
  *ddest = (*dsrc1 < *dsrc2) ? 1 : 0;
}

def_EHelper(xor) {
  rtl_xor(s, ddest, dsrc1, dsrc2);
}

def_EHelper(or) {
  rtl_or(s, ddest, dsrc1, dsrc2);
}

def_EHelper(and) {
  rtl_and(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sll) {
  rtl_sll(s, ddest, dsrc1, dsrc2);
}

def_EHelper(srl) {
  rtl_srl(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sra) {
  rtl_sra(s, ddest, dsrc1, dsrc2);
}

def_EHelper(mulu_lo) {
  rtl_mulu_lo(s, ddest, dsrc1, dsrc2);
}
def_EHelper(mulu_hi) {
  rtl_mulu_hi(s, ddest, dsrc1, dsrc2);
}
def_EHelper(muls_hi) {
  rtl_muls_hi(s, ddest, dsrc1, dsrc2);
}

def_EHelper(divu_q) {
  rtl_divu_q(s, ddest, dsrc1, dsrc2);
}
def_EHelper(divu_r) {
  rtl_divu_r(s, ddest, dsrc1, dsrc2);
}
def_EHelper(divs_q) {
  rtl_divs_q(s, ddest, dsrc1, dsrc2);
}
def_EHelper(divs_r) {
  rtl_divs_r(s, ddest, dsrc1, dsrc2);
}
