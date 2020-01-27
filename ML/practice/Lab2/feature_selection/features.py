import numpy as np

def get_matrix(a):
    return np.reshape(a, (28, 28))

def vertical_symmetry_feature(a):
    """Substracts left part of image from the right part and calculates mean of absolute differencies.
    More symmetrical digits will have lower value."""
    a = get_matrix(a)
    b = np.abs(a[:,:14] - a[:,14:])
    return np.mean(b)

def horizontal_symmetry_feature(a):
    """Substracts upper part of image from the lower part and calculates mean of absolute differencies.
    More symmetrical digits will have lower value."""
    a = get_matrix(a)
    b = np.abs(a[:14,:] - a[14:,:])
    return np.mean(b)

def horizontal_axis_feature(a):
    """Calculates mean of horizontal axis pixels.
    More horizontal-oriented digits will have higher value. Very good for 1."""
    a = get_matrix(a)
    return np.mean(a[10:18])

def vertical_axis_feature(a):
    """Calculates mean of vertical axis pixels.
    More vertical-oriented digits will have higher value. Very good for 4."""
    a = get_matrix(a)
    return np.mean(a[:,8:20])

def upper_part_feature(a):
    """Calculates mean of image upper part pixels.
    Digits with high pixels intensity in upper part of image will have higher value. Good for 0, 2, 9."""
    a = get_matrix(a)
    return np.mean(a[:14])

def lower_part_feature(a):
    """Calculates mean of image down part pixels.
    Digits with high pixels intensity in down part of image will have higher value. Good for 0, 1, 2, 6, 8."""
    a = get_matrix(a)
    return np.mean(a[14:])

def very_upper_part_feature(a):
    """Calculates mean of image very upper part pixels.
    Digits with high pixels intensity in very upper part of image will have higher value. Very good for 7, 5."""
    a = get_matrix(a)
    return np.mean(a[:6])

def very_very_upper_part_feature(a):
    """Calculates mean of image very upper part pixels.
    Digits with high pixels intensity in very upper part of image will have higher value. Used for one specific case found empirically."""
    a = get_matrix(a)
    return np.mean(a[:4])

def very_lower_part_feature(a):
    """Calculates mean of image very down part pixels.
    Digits with high pixels intensity in very down part of image will have higher value. Sometimes very good for 2."""
    a = get_matrix(a)
    return np.mean(a[23:])

def left_part_feature(a):
    """Calculates mean of image left part pixels.
    Digits with high pixels intensity in left part of image will have higher value. Good for 5."""
    a = get_matrix(a)
    return np.mean(a[:,:14])

def right_part_feature(a):
    """Calculates mean of image right part pixels.
    Digits with high pixels intensity in right part of image will have higher value. Good for 2, 3, 9."""
    a = get_matrix(a)
    return np.mean(a[:,14:])

def сutted_right_part_feature(a):
    """Calculates mean of image right part pixels.
    Digits with high pixels intensity in right part of image will have higher value. Good for specific cases with 3 and 6."""
    a = get_matrix(a)
    return np.mean(a[6:22,14:])

def upper_right_part_feature(a):
    """Calculates mean of image upper right part pixels.
    Digits with high pixels intensity in right part of image will have higher value. Good for 2, 8. Good against 6, 5."""
    a = get_matrix(a)
    return np.mean(a[:14,14:])

def upper_left_part_feature(a):
    """Calculates mean of image upper left part pixels.
    Digits with high pixels intensity in right part of image will have higher value. Good for ...."""
    a = get_matrix(a)
    return np.mean(a[:14, :14])

def left_center_feature(a):
    """Calculates mean of left center image part pixels.
    Digits with left center  filling will have higher value. Good for 4, 6."""
    a = get_matrix(a)
    return np.mean(a[8:20,:14])

def center_feature(a):
    """Calculates mean of center image pixels.
    Digits with filled center will have higher value. Good against 0."""
    a = get_matrix(a)
    return np.mean(a[10:18,10:18])

def small_center_feature(a):
    """Calculates mean of center image pixels.
    Digits with filled center will have higher value. Very good against 0."""
    a = get_matrix(a)
    return np.mean(a[12:16,12:16])

def narrow_center_feature(a):
    """Calculates mean of center image pixels.
    Digits with filled center will have higher value. Very good against 0."""
    a = get_matrix(a)
    return np.mean(a[14:16,14:16])

def vertical_center_feature(a):
    """Calculates mean of vertical center pixels.
    Vertical-oriented digits will have higher value. Good for 1, 3, 7, 8"""
    a = get_matrix(a)
    return np.mean(a[8:20, 10:18])

def horizontal_center_feature(a):
    """Calculates mean of horizontal center pixels.
    Horizontal-oriented digits will have higher value. Good for 4, 6, 8, 9."""
    a = get_matrix(a)
    return np.mean(a[10:18, 8:20])

FEATURES = {
    (0, 1) : (vertical_axis_feature, narrow_center_feature),
    (0, 2) : (upper_part_feature, narrow_center_feature),
    (0, 3) : (horizontal_symmetry_feature, small_center_feature),
    (0, 4) : (upper_part_feature, narrow_center_feature),
    (0, 5) : (horizontal_symmetry_feature, small_center_feature),
    (0, 6) : (upper_part_feature, narrow_center_feature),
    (0, 7) : (vertical_center_feature, lower_part_feature),
    (0, 8) : (horizontal_symmetry_feature, narrow_center_feature),
    (0, 9) : (lower_part_feature, narrow_center_feature),
    (1, 2) : (vertical_axis_feature, lower_part_feature),
    (1, 3) : (horizontal_symmetry_feature, vertical_center_feature),
    (1, 4) : (horizontal_axis_feature, left_center_feature),
    (1, 5) : (vertical_axis_feature, lower_part_feature),
    (1, 6) : (vertical_axis_feature, lower_part_feature),
    (1, 7) : (horizontal_symmetry_feature, very_upper_part_feature),
    (1, 8) : (vertical_axis_feature, vertical_symmetry_feature),
	(1, 9) : (vertical_center_feature, horizontal_center_feature),
    (2, 3) : (lower_part_feature, right_part_feature),
    (2, 4) : (vertical_axis_feature, horizontal_axis_feature),
    (2, 5) : (upper_part_feature, right_part_feature),
    (2, 6) : (horizontal_center_feature, upper_right_part_feature),
    (2, 7) : (upper_part_feature, lower_part_feature),
    (2, 8) : (upper_part_feature, lower_part_feature),
    (2, 9) : (upper_part_feature, very_lower_part_feature),
    (3, 4) : (horizontal_axis_feature, vertical_symmetry_feature),
    (3, 5) : (right_part_feature, left_part_feature),
    (3, 6) : (horizontal_axis_feature, vertical_symmetry_feature),
    (3, 7) : (vertical_center_feature, very_upper_part_feature),
    (3, 8) : (vertical_center_feature, сutted_right_part_feature),
    (3, 9) : (horizontal_axis_feature, vertical_symmetry_feature),
    (4, 5) : (horizontal_axis_feature, vertical_symmetry_feature),
    (4, 6) : (horizontal_axis_feature, vertical_symmetry_feature),
    (4, 7) : (horizontal_center_feature, very_upper_part_feature),
    (4, 8) : (horizontal_axis_feature, vertical_symmetry_feature),
    (4, 9) : (horizontal_axis_feature, upper_part_feature),
    (5, 6) : (very_very_upper_part_feature, vertical_symmetry_feature),
    (5, 7) : (very_upper_part_feature, left_part_feature),
    (5, 8) : (center_feature, vertical_symmetry_feature),
	(5, 9) : (horizontal_center_feature, left_part_feature),
    (6, 7) : (lower_part_feature, сutted_right_part_feature),
    (6, 8) : (left_center_feature, vertical_symmetry_feature),
    (6, 9) : (lower_part_feature, сutted_right_part_feature),
    (7, 8) : (very_upper_part_feature, horizontal_center_feature),
    (7, 9) : (upper_part_feature, horizontal_center_feature),
    (8, 9) : (lower_part_feature, horizontal_center_feature),
}
