! -----------------------------------------------------------------------
subroutine write_f (rank, length, rdata)

  implicit none
  integer rank, length, u, i
  integer, parameter:: dp=kind(0.d0)
  real(dp), intent(in) :: rdata(length)

  !write(*,*) "Write from Fortran!", rank, length, rdata(1)

  open(newunit=u,file='test.fbin',form='unformatted',status='replace')
  write(u) rdata !(rdata(i), i=1,length)
  flush(u)
  close(u)

end subroutine write_f



! -----------------------------------------------------------------------
subroutine read_f (rank, length, rdata)

  implicit none
  integer rank, length, u
  integer, parameter:: dp=kind(0.d0)
  real(dp), intent(out) :: rdata(length)

  !write(*,*) "Read  from Fortran!", rank, length

  open(newunit=u,file='test.fbin',form='unformatted',status='old')
  read(u) rdata !(rdata(i), i=1,length)
  close(u)

end subroutine read_f
