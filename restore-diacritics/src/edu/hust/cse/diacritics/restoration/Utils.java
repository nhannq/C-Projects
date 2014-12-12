package edu.hust.cse.diacritics.restoration;

public class Utils {
	public static String removeDiacritics(String s) {
		s = s.replaceAll("\341", "a");
		s = s.replaceAll("\340", "a");
		s = s.replaceAll("\u1EA3", "a");
		s = s.replaceAll("\u00E3", "a");
		s = s.replaceAll("\u1EA1", "a");
		s = s.replaceAll("\342", "a");
		s = s.replaceAll("\u1EA5", "a");
		s = s.replaceAll("\u1EA7", "a");
		s = s.replaceAll("\u1EA9", "a");
		s = s.replaceAll("\u1EAB", "a");
		s = s.replaceAll("\u1EAD", "a");
		s = s.replaceAll("\u0103", "a");
		s = s.replaceAll("\u1EAF", "a");
		s = s.replaceAll("\u1EB1", "a");
		s = s.replaceAll("\u1EB3", "a");
		s = s.replaceAll("\u1EB5", "a");
		s = s.replaceAll("\u1EB7", "a");
		s = s.replaceAll("\351", "e");
		s = s.replaceAll("\350", "e");
		s = s.replaceAll("\u1EBB", "e");
		s = s.replaceAll("\u1EBD", "e");
		s = s.replaceAll("\u1EB9", "e");
		s = s.replaceAll("\352", "e");
		s = s.replaceAll("\u1EBF", "e");
		s = s.replaceAll("\u1EC1", "e");
		s = s.replaceAll("\u1EC3", "e");
		s = s.replaceAll("\u1EC5", "e");
		s = s.replaceAll("\u1EC7", "e");
		s = s.replaceAll("\363", "o");
		s = s.replaceAll("\362", "o");
		s = s.replaceAll("\u1ECF", "o");
		s = s.replaceAll("\365", "o");
		s = s.replaceAll("\u1ECD", "o");
		s = s.replaceAll("\364", "o");
		s = s.replaceAll("\u1ED3", "o");
		s = s.replaceAll("\u1ED1", "o");
		s = s.replaceAll("\u1ED5", "o");
		s = s.replaceAll("\u1ED7", "o");
		s = s.replaceAll("\u1ED9", "o");
		s = s.replaceAll("\u01A1", "o");
		s = s.replaceAll("\u1EDB", "o");
		s = s.replaceAll("\u1EDD", "o");
		s = s.replaceAll("\u1EDF", "o");
		s = s.replaceAll("\u1EE1", "o");
		s = s.replaceAll("\u1EE3", "o");
		s = s.replaceAll("\355", "i");
		s = s.replaceAll("\354", "i");
		s = s.replaceAll("\u1EC9", "i");
		s = s.replaceAll("\u0129", "i");
		s = s.replaceAll("\u1ECB", "i");
		s = s.replaceAll("\u00EC", "i");
		s = s.replaceAll("\372", "u");
		s = s.replaceAll("\371", "u");
		s = s.replaceAll("\u1EE7", "u");
		s = s.replaceAll("\u0169", "u");
		s = s.replaceAll("\u01B0", "u");
		s = s.replaceAll("\u1EE9", "u");
		s = s.replaceAll("\u1EEB", "u");
		s = s.replaceAll("\u1EED", "u");
		s = s.replaceAll("\u00FA", "u");
		s = s.replaceAll("\u1EEF", "u");
		s = s.replaceAll("\u00F9", "u");
		s = s.replaceAll("\u1EF1", "u");
		s = s.replaceAll("\u0111", "d");
		s = s.replaceAll("\375", "y");
		s = s.replaceAll("\u1EF3", "y");
		s = s.replaceAll("\u1EF7", "y");
		s = s.replaceAll("\u1EF9", "y");
		s = s.replaceAll("\u1EF5", "y");
		return s;
	}

	public static double min(double arr[]) {
		double min = arr[0];
		for (int i = 1; i < arr.length; i++) {
			double so = arr[i];
			if (so != 0.0D)
				min = min > so ? so : min;
		}
		return min;
	}

	public static int getPosInArr(double so, double arr[]) {
		for (int i = 0; i < arr.length; i++)
			if (so == arr[i])
				return i;
		return -1;
	}

	public static boolean moveArr(double arr1[], double arr2[]) {
		if (arr1.length < arr2.length)
			return false;
		for (int i = 0; i < arr2.length; i++) {
			arr1[i] = arr2[i];
			arr2[i] = 0.0D;
		}
		return true;
	}
}
