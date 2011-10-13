function sortDictByStrAsc(a,b)
{
	if (a.value < b.value)
	{
		return -1;
	}

	else if (a.value > b.value)
	{
		return 1;
	}

	else
	{
		return 0;
	}
}

function sortDictByStrDesc(a,b)
{
	return -sortDictByStrAsc(a,b);
}

function sortDictByNumAsc(a,b)
{
	return a.value-b.value;
}

function sortDictByNumDesc(a,b)
{
	return b.value-a.value;
}

function orderColumn(tableID, colNum, isString)
{
	var colVals = new Array();
	var rows = document.getElementById(tableID).getElementsByTagName('tr');
	var selectedCell = rows[0].cells[colNum];

	//Set the ASC/DESC value of the selected row
	if (!selectedCell.isDescending || (selectedCell.isDescending == undefined))
	{
		selectedCell.isDescending = true;

		//Point arrow up
		selectedCell.className = "TABLE_DESC";
	}
	else
	{
		selectedCell.isDescending = false;

		//Point arrow down
		selectedCell.className = "TABLE_ASC";
	}

	//Set the ASC/DESC values
	for (var i=0; i < rows[0].cells.length; i++)
	{
		cell = rows[0].cells[i];

		if (i != colNum)
		{
			cell.isDescending = true;

			//Remove arrow
			cell.className = "TABLE_NONE";
		}
	}

	//Get all the values of the column
	for (var i=1; i < rows.length; i++)
	{
		cell = new Object();
		cell.key = i;
		cell.value = rows[i].cells[colNum].innerHTML;
		colVals[i-1] = cell;
	}

	//Sort the column
	if (isString) //The column contains string values
	{
		if (selectedCell.isDescending)
		{
			colVals = colVals.sort(sortDictByStrDesc);
		}

		else
		{
			colVals = colVals.sort(sortDictByStrAsc);
		}
	}

	else
	{
		if (selectedCell.isDescending)
		{
			colVals = colVals.sort(sortDictByNumDesc);
		}

		else
		{
			colVals = colVals.sort(sortDictByNumAsc);
		}
	}

	//Reorder the column
	for (var i=1; i < rows.length; i++)
	{
		newIndex = colVals[i-1].key;

		//Replace the values in the rows 
		for (var j=0; j < rows[i].cells.length; j++)
		{
			var oldCell = rows[i].cells[j];
			var newCell = rows[newIndex].cells[j];
			var oldContent = oldCell.innerHTML;
			oldCell.innerHTML = newCell.innerHTML;
			newCell.innerHTML = oldContent;
		}

		// Find the old row value in the array and replace
		// it with the index of the new location
		var index;
		for (var index=colVals.length-1; (index >= 0) && (colVals[index].key != i); index--) ;
		colVals[index].key = newIndex;
	}
}