package edu.hust.cse.diacritics.restoration;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.Hashtable;
import java.util.StringTokenizer;

public class RestoreDiacritics {
	private static String MAP_FILE = "data/map.txt";
	private static String BIGRAM_FILE = "data/bigram.txt";
	private static int PLEN = 150;
	private static int NB_ELEMENTS = 1000;
	static Hashtable<String, String[]> htMap;
	static Hashtable<String, Double> htProbability;

	public static void init() {
		htMap = new Hashtable<String, String[]>();
		htProbability = new Hashtable<String, Double>();
		loadDataBase();
	}

	public static int loadDataBase() {
		String s = new String();
		try {
			BufferedReader br = new BufferedReader(new InputStreamReader(
					new FileInputStream(MAP_FILE), "UTF-16"));
			while ((s = br.readLine()) != null) {
				String temp[] = s.split("\\|");
				if (temp.length == 2)
					htMap.put(temp[0], temp[1].split(","));
			}
			br.close();
			br = new BufferedReader(new InputStreamReader(new FileInputStream(
					BIGRAM_FILE), "UTF-16"));
			while ((s = br.readLine()) != null) {
				String temp[] = s.split("\\|");
				if (temp.length == 2)
					htProbability.put(temp[0],
							Double.valueOf(Double.parseDouble(temp[1])));
			}
			br.close();
		} catch (Exception ex) {
			System.out.print((new StringBuilder("Cannot loadDataBase: "))
					.append(ex).toString());
		}
		return 1;
	}

	public static String restore(String str) {
		String s = new String();
		double pNotHas = htProbability.get("notHas").doubleValue();
		double p1[] = new double[PLEN];
		double p2[] = new double[PLEN];
		int track[][] = new int[NB_ELEMENTS][PLEN];
		str = str.trim();
		String word[] = str.split("[ ]+");
		String wordResult[] = str.toLowerCase().split("[ ]+");
		if (!htMap.containsKey(wordResult[0]))
			htMap.put(wordResult[0], wordResult[0].split("\\s"));
		String temp1[] = htMap.get(wordResult[0]);
		for (int i = 1; i < wordResult.length; i++) {
			if (!htMap.containsKey(wordResult[i]))
				htMap.put(wordResult[i], wordResult[i].split("\\s"));
			String temp2[] = (String[]) htMap.get(wordResult[i]);
			for (int j = 0; j < temp2.length; j++) {
				double ptemp[] = new double[150];
				for (int k = 0; k < temp1.length; k++) {
					s = new String(
							(new StringBuilder(String.valueOf(temp1[k])))
									.append(" ").append(temp2[j]).toString());
					if (htProbability.containsKey(s))
						ptemp[k] = p1[k] + htProbability.get(s).doubleValue();
					else
						ptemp[k] = p1[k] + pNotHas;
				}
				p2[j] = Utils.min(ptemp);
				int pos = Utils.getPosInArr(p2[j], ptemp);
				track[i][j] = i * 10 + pos;
			}
			Utils.moveArr(p1, p2);
			temp1 = temp2;
		}
		int pos = Utils.getPosInArr(Utils.min(p1), p1);
		for (int i = wordResult.length - 1; i >= 0; i--) {
			temp1 = (String[]) htMap.get(wordResult[i]);
			wordResult[i] = temp1[pos];
			pos = track[i][pos] % 10;
		}
		s = new String();
		for (int i = 0; i < wordResult.length; i++) {
			String singleWord = wordResult[i];
			if (!Utils.removeDiacritics(singleWord).equals(word[i])) {
				char charArrWord[] = singleWord.toCharArray();
				singleWord = new String();
				for (int j = 0; j < charArrWord.length; j++) {
					String ch = new String(charArrWord, j, 1);
					if (word[i].charAt(j) > '@' && word[i].charAt(j) < '[')
						ch = ch.toUpperCase();
					singleWord = (new StringBuilder(String.valueOf(singleWord)))
							.append(ch).toString();
				}

				wordResult[i] = singleWord;
			}
			s = (new StringBuilder(String.valueOf(s))).append(wordResult[i])
					.append(" ").toString();
		}
		return s.trim();
	}

	public static String restoreTextSegment(String textSegment) {
		StringTokenizer strTokenizer = new StringTokenizer(textSegment,
				"()[]/-\"\u2026',\u201C\u201D\u2018\u2019?:;", true);
		String tempStr;
		for (textSegment = new String(); strTokenizer.hasMoreTokens(); textSegment = (new StringBuilder(
				String.valueOf(textSegment))).append(tempStr).toString()) {
			boolean check = false;
			tempStr = strTokenizer.nextToken();
			if (tempStr.equals(".") || tempStr.equals(",")
					|| tempStr.equals("?") || tempStr.equals("!")
					|| tempStr.equals(":") || tempStr.equals(")")
					|| tempStr.equals("]") || tempStr.equals("\u201D")
					|| tempStr.equals("\u2019") || tempStr.equals("\u2026")) {
				check = true;
			}
			if (tempStr.equals("(") || tempStr.equals("[")
					|| tempStr.equals("\u201C") || tempStr.equals("\u2018")) {
				check = true;
			}
			if (tempStr.equals("-")) {
				check = true;
			}
			if (tempStr.equals("\"") || tempStr.equals("'")
					|| tempStr.equals("\n") || tempStr.equals("/"))
				check = true;
			if (!check)
				tempStr = " " + restore(tempStr) + " ";
		}
		return textSegment;
	}
}