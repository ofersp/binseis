all: EOS5_xtrin EOS5_xtpin ZFS_interp_EOS5
default: all

EOS5_xtrin : EOS5_xtrin.f
	gfortran -g EOS5_xtrin.f -o EOS5_xtrin

EOS5_xtpin : EOS5_xtpin.f
	gfortran -g EOS5_xtpin.f -o EOS5_xtpin

ZFS_interp_EOS5 : ZFS_interp_EOS5.f
	gfortran -g ZFS_interp_EOS5.f -o ZFS_interp_EOS5

clean : 
	rm -f core EOS5_xtrin EOS5_xtpin ZFS_interp_EOS5

