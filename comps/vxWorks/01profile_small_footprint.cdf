/* 01profile_small_footprint.cdf - profile tuned for a small RAM image */

Profile PROFILE_SMALL_FOOTPRINT {
    NAME	Profile is tuned for a small RAM image
    SYNOPSIS	Using this profile will tune the configuration with the goal of producing the smallest possible VxWorks RAM footprint image.
    COMPONENTS	INCLUDE_KERNEL		\
		INCLUDE_NO_INTRINSICS	\
                INCLUDE_SYSCLK_INIT
    PROFILES    BSP_PARAMETER_VALUES
}
